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
	int part = 6;
	GLuint FitTextureW = 2048;
	GLuint FitTextureH = 2048;
	glm::vec4 color;
	int VertexNum = 0;

public:
	void Init(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, glm::vec4 c)
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
	TransparentPlane(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, glm::vec4 c) 
		:ViewObject(ViewObjectEnum::TransparentObject)
	{
		this->Init(position, points, sizeofPoints, indices, sizeofIndices, c);
	}
	TransparentPlane(std::vector<GLfloat> points, std::vector<GLuint> indices, glm::vec4 color, glm::vec3 position = glm::vec3(0.0f))
		:ViewObject(ViewObjectEnum::TransparentObject)
	{
		this->Init(position, points.data(), (GLint)points.size() * sizeof(GLfloat) ,
			indices.data(), (GLint)indices.size() * sizeof(GLuint), color);
	}
	void SetColor(const glm::vec4& rgbaColor)
	{
		this->color = rgbaColor;
	}
	virtual void RenderOITBuffer();

};