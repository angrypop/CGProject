#pragma once
#include "ViewHeader.h"
#include "toolkit.h"
namespace Shadow
{
	enum { ShadowBuffer, NumShadowTextureBuffer };
	enum { ShadowFrameBuffer, NumShadowFrameBuffer };
	enum { PlaneProgram, WaterProgram, GrassProgram, DisplayShadowProgram, FitPlaneShadowProgram, NumShadowProgram };
	extern GLuint shadowBuffers[NumShadowTextureBuffer];
	extern GLuint shadowFrameBuffers[NumShadowFrameBuffer];
	extern GLuint shadowPrograms[NumShadowProgram];
	const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
}


void InitShadow();