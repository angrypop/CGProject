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
	{
		this->Init(position, points, sizeofPoints, indices, sizeofIndices, shaders, textures);
	}

	TexturedPlane(std::vector<GLfloat> vertices)
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

	virtual void RenderGrassGBuffer() {
		glm::mat4 cameraView = GlobalDataPool::GetData<glm::mat4>("cameraView");
		GLfloat uniTime = GlobalDataPool::GetData<GLfloat>("systemTime");
		glm::vec3 ObjPos = GlobalDataPool::GetData<glm::vec3>("cameraPosition");
		glm::mat4 cameraProjection = GlobalDataPool::GetData<glm::mat4>("cameraProjection");
		GLint location;
		glUseProgram(GrassProgram);

		location = glGetUniformLocation(GrassProgram, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
		location = glGetUniformLocation(GrassProgram, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraView));
		location = glGetUniformLocation(GrassProgram, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraProjection));
		location = glGetUniformLocation(GrassProgram, "uniTime");
		glUniform1f(location, uniTime);
		location = glGetUniformLocation(GrassProgram, "uniObjPos");
		glUniform3fv(location, 1, glm::value_ptr(ObjPos));
		glBindVertexArray(this->VAOs[PlaneVAO]);


		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4);
		glUseProgram(0);
	}

	virtual void RenderGBuffer() {
		if (this->renderGrassFlag)
		{
			this->RenderGrassGBuffer();
		}
		glm::mat4 uniP = GlobalDataPool::GetData<glm::mat4>("cameraProjection");
		glm::mat4 uniV = GlobalDataPool::GetData<glm::mat4>("cameraView");
		GLint location;
		glUseProgram(GBufferProgram);

		location = glGetUniformLocation(GBufferProgram, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
		location = glGetUniformLocation(GBufferProgram, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(GBufferProgram, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(GBufferProgram, "TBN");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(this->TBN));
		location = glGetUniformLocation(GBufferProgram, "colorTexture");
		glUniform1i(location, PlaneColorTexture);
		location = glGetUniformLocation(GBufferProgram, "normalTexture");
		glUniform1i(location, PlaneNormalTexture);
		location = glGetUniformLocation(GBufferProgram, "roughnessTexture");
		glUniform1i(location, PlaneRoughnessTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->Textures[PlaneColorTexture]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->Textures[PlaneNormalTexture]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->Textures[PlaneRoughnessTexture]);


		glBindVertexArray(this->VAOs[PlaneVAO]);
		glDrawElements(GL_TRIANGLES, this->VertexNum, GL_UNSIGNED_INT, 0);
		glUseProgram(0);
	}

	virtual void RenderShadowGrassBuffer();

	virtual void RenderShadowBuffer();

	void prepare(Light& light) {
		GLuint location;

		glUseProgram(Program);
		//location = glGetUniformLocation(Program, "ambientStrength");
		//glUniform1f(location, ambientStrength);
		location = glGetUniformLocation(Program, "lightColor");
		glUniform3fv(location, 1, glm::value_ptr(light.LightColor));
		location = glGetUniformLocation(Program, "colorTexture");
		glUniform1i(location, PlaneColorTexture);
		location = glGetUniformLocation(Program, "normalTexture");
		glUniform1i(location, PlaneNormalTexture);
		location = glGetUniformLocation(Program, "roughnessTexture");
		glUniform1i(location, PlaneRoughnessTexture);
		location = glGetUniformLocation(Program, "light.ambient");
		glUniform3fv(location, 1, glm::value_ptr(light.ambient));
		location = glGetUniformLocation(Program, "light.diffuse");
		glUniform3fv(location, 1, glm::value_ptr(light.diffuse));
		location = glGetUniformLocation(Program, "light.specular");
		glUniform3fv(location, 1, glm::value_ptr(light.specular));
		location = glGetUniformLocation(Program, "light.constant");
		glUniform1f(location, light.constant);
		location = glGetUniformLocation(Program, "light.linear");
		glUniform1f(location, light.linear);
		location = glGetUniformLocation(Program, "light.quadratic");
		glUniform1f(location, light.quadratic);
		location = glGetUniformLocation(Program, "TBN");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(this->TBN));
	}
};