#pragma once

#include "ViewObject.h"
#include "GlobalData.h"
#include "Light.h"
#include "Camera.h"

class TransparentPlane  : public ViewObject{
public:
	enum { PlaneVAO, NumPlaneVAO };
	enum { PlaneArrayBuffer, PlaneElementBuffer, NumPlaneBuffer };
	enum { position = 0, texCoord = 1 };

	GLuint VAOs[NumPlaneVAO];
	GLuint Buffers[NumPlaneBuffer];
	//GLuint Program;
	//GLuint GBufferProgram;
	//GLuint GrassProgram;
	//GLuint updateProgram;
	//glm::mat3 TBN;

	int part = 6;
	GLuint FitTextureW = 2048;
	GLuint FitTextureH = 2048;
	//GLuint fitVAO;
	//GLuint fitVBO;
	//GLuint fitEBO;

	glm::vec4 color;

	int VertexNum = 0;

public:
	//TransparentPlane() {};

	TransparentPlane(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, glm::vec4 c) 
	{
		color = c;
		VertexNum = sizeofIndices / sizeof(GLuint);
		glGenVertexArrays(NumPlaneVAO, this->VAOs);
		glCreateBuffers(NumPlaneBuffer, this->Buffers);

		glNamedBufferStorage(this->Buffers[PlaneArrayBuffer], sizeofPoints, points, 0);
		glNamedBufferStorage(this->Buffers[PlaneElementBuffer], sizeofIndices, indices, 0);

		glBindVertexArray(this->VAOs[PlaneVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, this->Buffers[PlaneArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Buffers[PlaneElementBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		modelMat = glm::translate(glm::mat4(1.0), position);
	}

	//virtual void RenderGBuffer(glm::mat4 uniV, glm::mat4 uniP) {
	//	GLint location;
	//	glUseProgram(GBufferProgram);

	//	location = glGetUniformLocation(GBufferProgram, "uniM");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	//	location = glGetUniformLocation(GBufferProgram, "uniV");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
	//	location = glGetUniformLocation(GBufferProgram, "uniP");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));

	//	glBindVertexArray(this->VAOs[PlaneVAO]);
	//	//glBindVertexArray(this->VAOs[FitTextureVAO]);
	//	glPatchParameteri(GL_PATCH_VERTICES, 4);
	//	glDrawArrays(GL_PATCHES, 0, 4 * part * part);
	//	//glBindVertexArray(this->VAOs[FitPlaneVAO]);
	//	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//}

	virtual void RenderOITBuffer();

	//virtual void RenderShadowBuffer(GLuint program, glm::mat4 uniV, glm::mat4 uniP) {
	//	GLint location;
	//	glUseProgram(program);

	//	location = glGetUniformLocation(program, "uniM");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	//	location = glGetUniformLocation(program, "uniV");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
	//	location = glGetUniformLocation(program, "uniP");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));

	//	glBindVertexArray(this->VAOs[PlaneVAO]);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//}

	//void prepare(Light& light) {
	//	GLuint location;

	//	glUseProgram(Program);
	//	//location = glGetUniformLocation(Program, "ambientStrength");
	//	//glUniform1f(location, ambientStrength);
	//	location = glGetUniformLocation(Program, "lightColor");
	//	glUniform3fv(location, 1, glm::value_ptr(light.LightColor));
	//	location = glGetUniformLocation(Program, "light.ambient");
	//	glUniform3fv(location, 1, glm::value_ptr(light.ambient));
	//	location = glGetUniformLocation(Program, "light.diffuse");
	//	glUniform3fv(location, 1, glm::value_ptr(light.diffuse));
	//	location = glGetUniformLocation(Program, "light.specular");
	//	glUniform3fv(location, 1, glm::value_ptr(light.specular));
	//	location = glGetUniformLocation(Program, "light.constant");
	//	glUniform1f(location, light.constant);
	//	location = glGetUniformLocation(Program, "light.linear");
	//	glUniform1f(location, light.linear);
	//	location = glGetUniformLocation(Program, "light.quadratic");
	//	glUniform1f(location, light.quadratic);
	//}
};