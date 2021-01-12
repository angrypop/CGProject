#pragma once
#include "ViewObject.h"

class WaterPlane : public ViewObject{
public:
	enum { WaterVAO, WaterMeshVAO, WaterNumVAO };
	enum { WaterArrayBuffer, WaterElementBuffer, WaterMeshElementBuffer, WaterNumBuffer };

	GLuint VAOs[WaterNumVAO];
	GLuint Buffers[WaterNumBuffer];
	GLuint Program;
	GLuint ProgramMesh;
	GLuint gBufferProgram;
	GLuint numTri;

public:
	WaterPlane() {}
	WaterPlane(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2, GLfloat y, GLuint xDivNum, GLuint zDivNum) : numTri(xDivNum* zDivNum * 2) {
		GLfloat* point = new GLfloat[(xDivNum + 1) * (zDivNum + 1) * 3];
		GLuint* index = new GLuint[3 * 2 * xDivNum * zDivNum];
		GLuint* indexLine = new GLuint[3 * 2 * xDivNum * zDivNum];

		for (int i = 0; i <= xDivNum; i++)
			for (int j = 0; j <= zDivNum; j++) {
				point[(i + j * (xDivNum + 1)) * 3 + 0] = (x2 - x1) * i / xDivNum + x1;
				point[(i + j * (xDivNum + 1)) * 3 + 1] = y;
				point[(i + j * (xDivNum + 1)) * 3 + 2] = (z2 - z1) * j / zDivNum + z1;
			}

		for (int i = 0; i < xDivNum; i++)
			for (int j = 0; j < zDivNum; j++) {
				index[(i + j * xDivNum) * 6 + 0] = i + j * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 1] = i + (j + 1) * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 2] = i + 1 + j * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 3] = i + 1 + j * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 4] = i + (j + 1) * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 5] = i + 1 + (j + 1) * (xDivNum + 1);
			}

		for (int i = 0; i < xDivNum; i++)
			for (int j = 0; j < zDivNum; j++) {
				indexLine[(i + j * xDivNum) * 6 + 0] = i + 1 + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 1] = i + 1 + j * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 2] = i + 1 + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 3] = i + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 4] = i + 1 + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 5] = i + j * (xDivNum + 1);
			}

		glGenVertexArrays(WaterNumVAO, VAOs);
		glCreateBuffers(WaterNumBuffer, Buffers);
		
		glNamedBufferStorage(Buffers[WaterArrayBuffer], (xDivNum + 1) * (zDivNum + 1) * 3 * sizeof(GLfloat), point, 0);
		glNamedBufferStorage(Buffers[WaterElementBuffer], 3 * 2 * xDivNum * zDivNum * sizeof(GLuint), index, 0);
		glNamedBufferStorage(Buffers[WaterMeshElementBuffer], 3 * 2 * xDivNum * zDivNum * sizeof(GLuint), indexLine, 0);

		glBindVertexArray(VAOs[WaterVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[WaterArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[WaterElementBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);

		Program = glCreateProgram();
		ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER,"waterV.shader"} ,
		{GL_FRAGMENT_SHADER,"waterF.shader"} ,
		{GL_NONE,""} };
		for (int i = 0; shaders[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shaders[i]);
			glAttachShader(Program, shader);
		}
		glLinkProgram(Program);

		glBindVertexArray(VAOs[WaterMeshVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[WaterArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[WaterMeshElementBuffer]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);

		ProgramMesh = glCreateProgram();
		ShaderInfo shadersMesh[] = {
		{GL_VERTEX_SHADER,"gBufferWaterV.shader"} ,
		{GL_FRAGMENT_SHADER,"WaterMeshF.shader"} ,
		{GL_NONE,""} };
		for (int i = 0; shadersMesh[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersMesh[i]);
			glAttachShader(ProgramMesh, shader);
		}
		glLinkProgram(ProgramMesh);

		gBufferProgram = glCreateProgram();
		ShaderInfo shadersGBuffer[] = {
		{GL_VERTEX_SHADER,"gBufferWaterV.shader"} ,
		{GL_FRAGMENT_SHADER,"gBufferWaterF.shader"} ,
		{GL_NONE,""} };
		for (int i = 0; shadersGBuffer[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersGBuffer[i]);
			glAttachShader(gBufferProgram, shader);
		}
		glLinkProgram(gBufferProgram);
	}

	void gBufferRender(glm::mat4 uniV, glm::mat4 uniP, float uniTime) {
		glUseProgram(gBufferProgram);

		GLint location;
		location = glGetUniformLocation(gBufferProgram, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(gBufferProgram, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(gBufferProgram, "uniTime");
		glUniform1f(location, uniTime);
		glBindVertexArray(this->VAOs[WaterVAO]);
		glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
	}
	void shadowBufferRender(GLuint program, glm::mat4 uniV, glm::mat4 uniP, float uniTime) {
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
		glBindVertexArray(this->VAOs[WaterVAO]);
		glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
	}
	void oitRender(GLuint program, glm::mat4 uniV, glm::mat4 uniP, float uniTime) {
		glUseProgram(program);

		GLint location;
		location = glGetUniformLocation(program, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(program, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(program, "uniTime");
		glUniform1f(location, uniTime);
		glBindVertexArray(this->VAOs[WaterVAO]);
		glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
	}
};