#pragma once
#include "ViewObject.h"

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

public:
	TexturedPlane() {};
	TexturedPlane(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, ShaderInfo shaders[], TextureInfo textures[]) {
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

		glGenTextures(NumPlaneTexture, this->Textures);
		for (int i = 0; textures[i].mode != Texture_NONE; i++) {
			this->hasTextures[textures[i].mode] = 1;
			glBindTexture(GL_TEXTURE_2D, this->Textures[textures[i].mode]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int width, height, nrChannels;
			unsigned char* data = stbi_load((textures[i].fileName).c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				switch (nrChannels) {
				case 1:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data); break;
				case 3:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); break;
				case 4:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); break;
				default:
					break;
				}
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				printf("fail to load image");
			}
		}
		modelMat = glm::translate(glm::mat4(1.0), position);

		glm::vec3 edge1(points[0*5+0] - points[1*5+0], points[0*5+1] - points[1*5+1], points[0*5+2] - points[1*5+2]);
		glm::vec3 edge2(points[2*5+0] - points[1*5+0], points[2*5+1] - points[1*5+1], points[2*5+2] - points[1*5+2]);
		glm::vec2 uv1(points[0*5+3] - points[1*5+3], points[0*5+4] - points[1*5+4]);
		glm::vec2 uv2(points[2*5+3] - points[1*5+3], points[2*5+4] - points[1*5+4]);
		GLfloat f = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
		glm::vec3 T(f * (uv2.y * edge1.x - uv1.y * edge2.x), f * (uv2.y * edge1.y - uv1.y * edge2.y), f * (uv2.y * edge1.z - uv1.y * edge2.z));
		T = glm::normalize(T);
		glm::vec3 B(f * (-uv2.x * edge1.x + uv1.x * edge2.x), f * (-uv2.x * edge1.y + uv1.x * edge2.y), f * (-uv2.x * edge1.z + uv1.x * edge2.z));
		B = glm::normalize(B);
		glm::vec3 N = glm::normalize(glm::cross(B,T));
		//printf("T(%f,%f,%f)\nB(%f,%f,%f)\nN(%f,%f,%f)\n", T.x, T.y, T.z, B.x, B.y, B.z, N.x, N.y, N.z);
		this->TBN = glm::mat3(T, B, N);

		ShaderInfo shadersGBuffer[] = {
           {GL_VERTEX_SHADER,"gBufferTexturePlaneV.shader"} ,
           {GL_FRAGMENT_SHADER,"gBufferTexturePlaneF.shader"} ,
           {GL_NONE,""} };

		GBufferProgram = glCreateProgram();
		for (int i = 0; shadersGBuffer[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersGBuffer[i]);
			glAttachShader(GBufferProgram, shader);
		}
		glLinkProgram(GBufferProgram);

		ShaderInfo shadersGrass[] = {
		{GL_VERTEX_SHADER,"gBufferTexturePlaneV.shader"} ,
		{GL_TESS_CONTROL_SHADER,"ControlT.shader"},
		{GL_TESS_EVALUATION_SHADER,"EvaluationT.shader"},
		{GL_GEOMETRY_SHADER,"NormalG.shader"},
		{GL_FRAGMENT_SHADER,"gBufferGrassF.shader"},
		//{GL_FRAGMENT_SHADER,"gBufferTexturePlaneF.shader"} ,
		{GL_NONE,""} };

		GrassProgram = glCreateProgram();
		for (int i = 0; shadersGrass[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersGrass[i]);
			glAttachShader(GrassProgram, shader);
		}
		glLinkProgram(GrassProgram);
	}

	void grassRender(glm::mat4 uniV, glm::mat4 uniP, float uniTime, glm::vec3 ObjPos) {
		GLint location;
		glUseProgram(GrassProgram);

		location = glGetUniformLocation(GrassProgram, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
		location = glGetUniformLocation(GrassProgram, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(GrassProgram, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(GrassProgram, "uniTime");
		glUniform1f(location, uniTime);
		location = glGetUniformLocation(GrassProgram, "ObjPos");
		glUniform3fv(location, 1, glm::value_ptr(ObjPos));
		glBindVertexArray(this->VAOs[PlaneVAO]);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4);
		//glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
	}

	void gBufferRender(glm::mat4 uniV, glm::mat4 uniP) {
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void shadowGrassBufferRender(GLuint program, glm::mat4 uniV, glm::mat4 uniP, float uniTime, glm::vec3 ObjPos) {
		GLint location;
		glUseProgram(program);

		location = glGetUniformLocation(program, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
		location = glGetUniformLocation(program, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(program, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(program, "uniTime");
		glUniform1f(location, uniTime);
		location = glGetUniformLocation(program, "ObjPos");
		glUniform3fv(location, 1, glm::value_ptr(ObjPos));
		glBindVertexArray(this->VAOs[PlaneVAO]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4);
	}

	void shadowBufferRender(GLuint program, glm::mat4 uniV, glm::mat4 uniP) {
		GLint location;
		glUseProgram(program);

		location = glGetUniformLocation(program, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
		location = glGetUniformLocation(program, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(program, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));

		glBindVertexArray(this->VAOs[PlaneVAO]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

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