#pragma once
#include "ViewObject.h"
#include "GlobalData.h"
#include "toolkit.h"
#include "Light.h"

class TexturedPlane : public ViewObject
{
public:
	enum { PlaneVAO, NumPlaneVAO };
	enum { PlaneArrayBuffer, PlaneElementBuffer, NumPlaneBuffer };
	enum { position = 0, texCoord = 1 };
	enum { PlaneColorTexture = AmbientTexture, PlaneNormalTexture = NormalTexture, PlaneRoughnessTexture = RoughnessTexture, NumPlaneTexture };

	GLuint VAOs[NumPlaneVAO];
	GLuint Buffers[NumPlaneBuffer];
	GLuint hasTextures[NumPlaneTexture];
	GLuint Textures[NumPlaneTexture];
	GLuint Program;
	GLuint GBufferProgram;
	GLuint GrassProgram;
	glm::mat3 TBN;
	bool renderGrassFlag = false;
	GLint VertexNum = 0;

public:
	//TexturedPlane() {};
	void Init(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, ShaderInfo shaders[], TextureInfo textures[])
	{
		this->VertexNum = sizeofIndices / sizeof(GLfloat);

		glGenVertexArrays(NumPlaneVAO, this->VAOs);
		glCreateBuffers(NumPlaneBuffer, this->Buffers);

		glNamedBufferStorage(this->Buffers[PlaneArrayBuffer], sizeofPoints, points, 0);
		glNamedBufferStorage(this->Buffers[PlaneElementBuffer], sizeofIndices, indices, 0);

		glBindVertexArray(this->VAOs[PlaneVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, this->Buffers[PlaneArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Buffers[PlaneElementBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		Program = glCreateProgram();
		for (int i = 0; shaders[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shaders[i]);
			glAttachShader(Program, shader);
		}
		glLinkProgram(Program);


		for (int i = 0; textures[i].mode != Texture_NONE; i++) {
			this->hasTextures[textures[i].mode] = 1;
			this->Textures[textures[i].mode] = LoadTexture(textures[i]);
		}
		modelMat = glm::translate(glm::mat4(1.0), position);

		glm::vec3 edge1(points[0 * 5 + 0] - points[1 * 5 + 0], points[0 * 5 + 1] - points[1 * 5 + 1], points[0 * 5 + 2] - points[1 * 5 + 2]);
		glm::vec3 edge2(points[2 * 5 + 0] - points[1 * 5 + 0], points[2 * 5 + 1] - points[1 * 5 + 1], points[2 * 5 + 2] - points[1 * 5 + 2]);
		glm::vec2 uv1(points[0 * 5 + 3] - points[1 * 5 + 3], points[0 * 5 + 4] - points[1 * 5 + 4]);
		glm::vec2 uv2(points[2 * 5 + 3] - points[1 * 5 + 3], points[2 * 5 + 4] - points[1 * 5 + 4]);
		GLfloat f = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
		glm::vec3 T(f * (uv2.y * edge1.x - uv1.y * edge2.x), f * (uv2.y * edge1.y - uv1.y * edge2.y), f * (uv2.y * edge1.z - uv1.y * edge2.z));
		T = glm::normalize(T);
		glm::vec3 B(f * (-uv2.x * edge1.x + uv1.x * edge2.x), f * (-uv2.x * edge1.y + uv1.x * edge2.y), f * (-uv2.x * edge1.z + uv1.x * edge2.z));
		B = glm::normalize(B);
		glm::vec3 N = glm::normalize(glm::cross(B, T));
		//printf("T(%f,%f,%f)\nB(%f,%f,%f)\nN(%f,%f,%f)\n", T.x, T.y, T.z, B.x, B.y, B.z, N.x, N.y, N.z);
		this->TBN = glm::mat3(T, B, N);

		GBufferProgram = GenerateProgram({
			{GL_VERTEX_SHADER, "GBufferTexture.vert"},
			{GL_FRAGMENT_SHADER,"GBufferTexture.frag" }
			});

		GrassProgram = GenerateProgram({
			{ GL_VERTEX_SHADER,"GBufferTexture.vert" },
			{ GL_TESS_CONTROL_SHADER,"GrassControl.tesc" },
			{ GL_TESS_EVALUATION_SHADER,"GrassEvaluation.tese" },
			{ GL_GEOMETRY_SHADER,"GrassNormal.geom" },
			{ GL_FRAGMENT_SHADER,"GBufferGrass.frag" }
			});
	}

	TexturedPlane(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, ShaderInfo shaders[], TextureInfo textures[]) 
		:ViewObject(ViewObjectEnum::TextureObject)
	{
		this->Init(position, points, sizeofPoints, indices, sizeofIndices, shaders, textures);
	}

	TexturedPlane(std::vector<GLfloat> vertices)
		:ViewObject(ViewObjectEnum::TextureObject)
	{
		this->VertexNum = (int)vertices.size() / 5;
		std::vector<GLuint> indices;

		for (int i = 0; i < VertexNum; i++)
		{
			indices.push_back(i);
		}
		ShaderInfo planeShaders[] = {
	{GL_VERTEX_SHADER,"Ground.vert"} ,
	{GL_FRAGMENT_SHADER,"Ground.frag"} ,
	{GL_NONE,""} };
		TextureInfo planeTextures[] = {
			{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Color.jpg"},
			{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
			{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
			{Texture_NONE,""} };
		this->Init(glm::vec3(0.0f), vertices.data(), (int)vertices.size() * sizeof(GLfloat), 
			indices.data(), (int)indices.size() * sizeof(GLfloat), planeShaders, planeTextures);
	}

	TexturedPlane(std::vector<GLfloat> vertices, std::vector<GLuint> indices, std::vector<TextureInfo> planeTextures)
		:ViewObject(ViewObjectEnum::TextureObject)
	{
		this->VertexNum = (int)indices.size();
		ShaderInfo planeShaders[] = {
	{GL_VERTEX_SHADER,"Ground.vert"} ,
	{GL_FRAGMENT_SHADER,"Ground.frag"} ,
	{GL_NONE,""} };
		
		this->Init(glm::vec3(0.0f), vertices.data(), (int)vertices.size() * sizeof(GLfloat),
			indices.data(), (int)indices.size() * sizeof(GLfloat), planeShaders, planeTextures.data());
	}

	virtual void RenderGrassGBuffer();

	virtual void RenderGBuffer();

	virtual void RenderShadowGrassBuffer();

	virtual void RenderShadowBuffer();

	void prepare(Light& light);
};