#pragma once

#include "ViewObject.h"

class FitTexturedPlane : public ViewObject
{
public:
	enum { FitPlaneVAO, FitTextureVAO, NumFitPlaneVAO };
	enum { FitFrameBuffer, NumFitFrameBuffer };
	enum { FitPlaneArrayBuffer, FitTextureArrayBuffer, FitTextureElementBuffer, FitHeightBuffer, FitTextureFrameBuffer, NumFitPlaneBuffer };
	enum { FitPlaneColorTexture = AmbientTexture, FitPlaneNormalTexture = NormalTexture, FitPlaneRoughnessTexture = RoughnessTexture, FitHeightTexture, NumFitPlaneTexture };

	GLuint VAOs[NumFitPlaneVAO];
	GLuint Buffers[NumFitPlaneBuffer];
	GLuint hasTextures[NumFitPlaneTexture];
	GLuint FrameBuffers[NumFitFrameBuffer];
	GLuint Textures[NumFitPlaneTexture];
	GLuint Program;
	GLuint GBufferProgram;
	GLuint GrassProgram;
	GLuint updateProgram;
	glm::mat3 TBN;

	int part = 6;
	GLuint FitTextureW = 2048;
	GLuint FitTextureH = 2048;
	//GLuint FitTextureW = 1280;
	//GLuint FitTextureH = 768;
	GLuint fitVAO;
	GLuint fitVBO;
	GLuint fitEBO;

public:
	FitTexturedPlane() {};
	FitTexturedPlane(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, ShaderInfo shaders[], TextureInfo textures[]) {
		//手动进行第一次细分 可以修改
		GLfloat* pointGen = new GLfloat[5 * 4 * part * part];
		for (int i = 0; i < part; i++)
			for (int j = 0; j < part; j++) 
				/*for (int k = 0; k < 5; k++) */{//for element
					// x y z s t
					pointGen[5 * 4 * (i * part + j) + 5 * 0 + 0] = (points[2 * 5 + 0] - points[0 * 5 + 0]) / part * i + points[0 * 5 + 0];
					pointGen[5 * 4 * (i * part + j) + 5 * 0 + 1] = (points[2 * 5 + 1] - points[0 * 5 + 1]) / part * i + points[0 * 5 + 1];
					pointGen[5 * 4 * (i * part + j) + 5 * 0 + 2] = (points[1 * 5 + 2] - points[0 * 5 + 2]) / part * j + points[0 * 5 + 2];
					pointGen[5 * 4 * (i * part + j) + 5 * 0 + 3] = (points[2 * 5 + 3] - points[0 * 5 + 3]) / part * i + points[0 * 5 + 3];
					pointGen[5 * 4 * (i * part + j) + 5 * 0 + 4] = (points[1 * 5 + 4] - points[0 * 5 + 4]) / part * j + points[0 * 5 + 4];

					// x y z s t
					pointGen[5 * 4 * (i * part + j) + 5 * 1 + 0] = (points[2 * 5 + 0] - points[0 * 5 + 0]) / part * (i + 1) + points[0 * 5 + 0];
					pointGen[5 * 4 * (i * part + j) + 5 * 1 + 1] = (points[2 * 5 + 1] - points[0 * 5 + 1]) / part * (i + 1) + points[0 * 5 + 1];
					pointGen[5 * 4 * (i * part + j) + 5 * 1 + 2] = (points[1 * 5 + 2] - points[0 * 5 + 2]) / part * j + points[0 * 5 + 2];
					pointGen[5 * 4 * (i * part + j) + 5 * 1 + 3] = (points[2 * 5 + 3] - points[0 * 5 + 3]) / part * (i + 1) + points[0 * 5 + 3];
					pointGen[5 * 4 * (i * part + j) + 5 * 1 + 4] = (points[1 * 5 + 4] - points[0 * 5 + 4]) / part * j + points[0 * 5 + 4];

					// x y z s t
					pointGen[5 * 4 * (i * part + j) + 5 * 2 + 0] = (points[2 * 5 + 0] - points[0 * 5 + 0]) / part * i + points[0 * 5 + 0];
					pointGen[5 * 4 * (i * part + j) + 5 * 2 + 1] = (points[2 * 5 + 1] - points[0 * 5 + 1]) / part * i + points[0 * 5 + 1];
					pointGen[5 * 4 * (i * part + j) + 5 * 2 + 2] = (points[1 * 5 + 2] - points[0 * 5 + 2]) / part * (j + 1) + points[0 * 5 + 2];
					pointGen[5 * 4 * (i * part + j) + 5 * 2 + 3] = (points[2 * 5 + 3] - points[0 * 5 + 3]) / part * i + points[0 * 5 + 3];
					pointGen[5 * 4 * (i * part + j) + 5 * 2 + 4] = (points[1 * 5 + 4] - points[0 * 5 + 4]) / part * (j + 1) + points[0 * 5 + 4];

					// x y z s t
					pointGen[5 * 4 * (i * part + j) + 5 * 3 + 0] = (points[2 * 5 + 0] - points[0 * 5 + 0]) / part * (i + 1) + points[0 * 5 + 0];
					pointGen[5 * 4 * (i * part + j) + 5 * 3 + 1] = (points[2 * 5 + 1] - points[0 * 5 + 1]) / part * (i + 1) + points[0 * 5 + 1];
					pointGen[5 * 4 * (i * part + j) + 5 * 3 + 2] = (points[1 * 5 + 2] - points[0 * 5 + 2]) / part * (j + 1) + points[0 * 5 + 2];
					pointGen[5 * 4 * (i * part + j) + 5 * 3 + 3] = (points[2 * 5 + 3] - points[0 * 5 + 3]) / part * (i + 1) + points[0 * 5 + 3];
					pointGen[5 * 4 * (i * part + j) + 5 * 3 + 4] = (points[1 * 5 + 4] - points[0 * 5 + 4]) / part * (j + 1) + points[0 * 5 + 4];

					//pointGen[5 * 4 * (i * d + j) + 5 * 0 + k] = (points[1 * 5 + k] - points[0 * 5 + k]) / d * (i + 1) + points[0 * 5 + k];
					//pointGen[5 * 4 * (i * d + j) + 5 * 1 + k] = (points[1 * 5 + k] - points[0 * 5 + k]) / d * (i + 1) + points[0 * 5 + k];
					//pointGen[5 * 4 * (i * d + j) + 5 * 2 + k] = (points[2 * 5 + k] - points[0 * 5 + k]) / d * (j + 1) + points[0 * 5 + k];
					//pointGen[5 * 4 * (i * d + j) + 5 * 3 + k] = (points[2 * 5 + k] - points[0 * 5 + k]) / d * (j + 1) + points[0 * 5 + k];
			}

		//for (int i = 0; i < d * d; i++) {
		//	for (int j = 0; j < 4; j++) {
		//		printf("[x:%f y:%f z:%f s:%f t:%f] ",
		//			pointGen[i * 4 * 5 + j * 5 + 0],
		//			pointGen[i * 4 * 5 + j * 5 + 1],
		//			pointGen[i * 4 * 5 + j * 5 + 2],
		//			pointGen[i * 4 * 5 + j * 5 + 3],
		//			pointGen[i * 4 * 5 + j * 5 + 4]);
		//	}
		//	puts("");
		//}

		glGenVertexArrays(NumFitPlaneVAO, this->VAOs);
		glCreateBuffers(NumFitPlaneBuffer, this->Buffers);

		glNamedBufferStorage(this->Buffers[FitPlaneArrayBuffer], 5 * 4 * part * part * sizeof(GLfloat), pointGen, 0);
		//glNamedBufferStorage(this->Buffers[FitPlaneElementBuffer], sizeofIndices, indices, 0);

		glBindVertexArray(this->VAOs[FitPlaneVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, this->Buffers[FitPlaneArrayBuffer]);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Buffers[FitPlaneElementBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//
		glNamedBufferStorage(this->Buffers[FitTextureArrayBuffer], sizeofPoints, points, 0);
		glNamedBufferStorage(this->Buffers[FitTextureElementBuffer], sizeofIndices, indices, 0);

		glBindVertexArray(this->VAOs[FitTextureVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, this->Buffers[FitTextureArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Buffers[FitTextureElementBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//glGenVertexArrays(1, &fitVAO);
		//glCreateBuffers(1, &fitVBO);
		//glCreateBuffers(1, &fitEBO);
		//glNamedBufferStorage(fitVBO, sizeofPoints, points, 0);
		//glNamedBufferStorage(fitEBO, sizeofIndices, indices, 0);

		//glBindVertexArray(fitVAO);
		//glBindBuffer(GL_ARRAY_BUFFER, fitVBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fitEBO);

		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(1);

		Program = glCreateProgram();
		for (int i = 0; shaders[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shaders[i]);
			glAttachShader(Program, shader);
		}
		glLinkProgram(Program);

		glGenTextures(NumFitPlaneTexture, this->Textures);
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
		glGenFramebuffers(NumFitFrameBuffer, FrameBuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffers[FitFrameBuffer]);

		glBindTexture(GL_TEXTURE_2D, Textures[FitHeightTexture]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, FitTextureW, FitTextureH, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Textures[FitHeightTexture], 0);

		GLuint attachments[1] = { GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, attachments);

		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FitTextureW, FitTextureH);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << " " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//depth
		//glGenBuffers(1, &Textures[FitHeightTexture]);
		//glGenFramebuffers(NumFitFrameBuffer, FrameBuffers);

		//glBindTexture(GL_TEXTURE_2D, Textures[FitHeightTexture]);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FitTextureW, FitTextureH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffers[FitFrameBuffer]);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Textures[FitHeightTexture], 0);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

		ShaderInfo shadersGBuffer[] = {
		   {GL_VERTEX_SHADER,"gBufferTexturePlaneV.shader"} ,
		{GL_TESS_CONTROL_SHADER,"PlaneCT.shader"},
		{GL_TESS_EVALUATION_SHADER,"PlaneET.shader"},
		{GL_GEOMETRY_SHADER,"PlaneGeo.shader"},
		   {GL_FRAGMENT_SHADER,"gBufferFitTexturePlaneF.shader"} ,
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

		ShaderInfo shadersUpdateFit[] = {
{GL_VERTEX_SHADER,"FitUpdateV.shader"} ,
{GL_FRAGMENT_SHADER,"FitUpdateF.shader"},
{GL_NONE,""} };
		updateProgram = glCreateProgram();
		for (int i = 0; shadersUpdateFit[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersUpdateFit[i]);
			glAttachShader(updateProgram, shader);
		}
		glLinkProgram(updateProgram);
	}

	void updateHeight(glm::vec3 ObjPos, glm::vec3 ObjVel,GLfloat radius, GLuint rst, glm::mat4 uniV, glm::mat4 uniP) {
		glViewport(0, 0, FitTextureW, FitTextureH);
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, this->FrameBuffers[FitFrameBuffer]);
		glClear(GL_DEPTH_BUFFER_BIT);
		GLint location;
		glUseProgram(updateProgram);

		glPointSize(10);

		location = glGetUniformLocation(GrassProgram, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
		location = glGetUniformLocation(GrassProgram, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(GrassProgram, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(updateProgram, "rst");
		glUniform1i(location, rst);
		location = glGetUniformLocation(updateProgram, "ObjPos");
		glUniform3fv(location, 1 , glm::value_ptr(ObjPos));
		location = glGetUniformLocation(updateProgram, "ObjVel");
		glUniform3fv(location, 1, glm::value_ptr(ObjVel));
		location = glGetUniformLocation(updateProgram, "radius");
		glUniform1f(location, radius);
		location = glGetUniformLocation(updateProgram, "heightMap");
		glUniform1i(location, FitHeightTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, this->Textures[FitHeightTexture]);

		glBindVertexArray(this->VAOs[FitTextureVAO]);
		//glPatchParameteri(GL_PATCH_VERTICES, 3);
		//glBindVertexArray(fitVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_POINTS, 0, 4);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glEnable(GL_DEPTH_TEST);
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
		glBindVertexArray(this->VAOs[FitPlaneVAO]);

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
		glUniform1i(location, FitPlaneColorTexture);
		location = glGetUniformLocation(GBufferProgram, "normalTexture");
		glUniform1i(location, FitPlaneNormalTexture);
		location = glGetUniformLocation(GBufferProgram, "roughnessTexture");
		glUniform1i(location, FitPlaneRoughnessTexture);
		location = glGetUniformLocation(GBufferProgram, "heightMap");
		glUniform1i(location, FitHeightTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->Textures[FitPlaneColorTexture]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->Textures[FitPlaneNormalTexture]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->Textures[FitPlaneRoughnessTexture]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, this->Textures[FitHeightTexture]);

		glBindVertexArray(this->VAOs[FitPlaneVAO]);
		//glBindVertexArray(this->VAOs[FitTextureVAO]);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4 * part * part);
		//glBindVertexArray(this->VAOs[FitPlaneVAO]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
		glBindVertexArray(this->VAOs[FitPlaneVAO]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4 * part * part);
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
		location = glGetUniformLocation(program, "heightMap");
		glUniform1i(location, FitHeightTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, this->Textures[FitHeightTexture]);

		glBindVertexArray(this->VAOs[FitPlaneVAO]);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4 * part * part);
	}

	void prepare(Light& light) {
		GLuint location;

		glUseProgram(Program);
		//location = glGetUniformLocation(Program, "ambientStrength");
		//glUniform1f(location, ambientStrength);
		location = glGetUniformLocation(Program, "lightColor");
		glUniform3fv(location, 1, glm::value_ptr(light.LightColor));
		location = glGetUniformLocation(Program, "colorTexture");
		glUniform1i(location, FitPlaneColorTexture);
		location = glGetUniformLocation(Program, "normalTexture");
		glUniform1i(location, FitPlaneNormalTexture);
		location = glGetUniformLocation(Program, "roughnessTexture");
		glUniform1i(location, FitPlaneRoughnessTexture);
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