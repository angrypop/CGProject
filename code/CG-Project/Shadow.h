#pragma once
#include "ViewHeader.h"
#include "toolkit.h"
//#define HighShadow
namespace Shadow
{
	enum { ShadowBuffer, NumShadowTextureBuffer };
	enum { ShadowFrameBuffer, NumShadowFrameBuffer };
	enum { PlaneProgram, WaterProgram, GrassProgram, DisplayShadowProgram, FitPlaneShadowProgram, NumShadowProgram };
	extern GLuint shadowBuffers[NumShadowTextureBuffer];
	extern GLuint shadowFrameBuffers[NumShadowFrameBuffer];
	extern GLuint shadowPrograms[NumShadowProgram];
#ifdef HighShadow
	const GLuint SHADOW_WIDTH = 16384, SHADOW_HEIGHT = 16384;
#else
	const GLuint SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
#endif // HighShadow

	
}


void InitShadow();