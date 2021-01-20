#pragma once
#include "ViewObject.h"
#include "GlobalData.h"
#include "toolkit.h"

class WaterPlane : public ViewObject
{
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
	WaterPlane(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2, GLfloat y, int xDivNum, int zDivNum);

	virtual void RenderGBuffer(glm::mat4 uniV, glm::mat4 uniP, float uniTime);
	virtual void RenderShadowBuffer();
	virtual void RenderOITBuffer();
};