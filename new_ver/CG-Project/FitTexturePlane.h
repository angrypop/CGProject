#pragma once

#include "ViewObject.h"
#include "GlobalData.h"
#include "Light.h"
#include "Shadow.h"

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
	//GLuint fitVAO;
	//GLuint fitVBO;
	//GLuint fitEBO;

public:
	//FitTexturedPlane() {};
	FitTexturedPlane(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, ShaderInfo shaders[], TextureInfo textures[]);

	void UpdateHeight(GLfloat radius, GLuint rst);

	virtual void RenderGrassGBuffer(glm::mat4 uniV, glm::mat4 uniP, float uniTime, glm::vec3 uniObjPos);

	virtual void RenderGBuffer();

	virtual void RenderShadowGrassBuffer(GLuint program, glm::mat4 uniV, glm::mat4 uniP, float uniTime, glm::vec3 uniObjPos);

	virtual void RenderShadowBuffer();

	void prepare(Light& light);
};