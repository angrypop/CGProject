#include "Shadow.h"
using namespace Shadow;
namespace Shadow
{
	GLuint shadowBuffers[NumShadowTextureBuffer];
	GLuint shadowFrameBuffers[NumShadowFrameBuffer];
	GLuint shadowPrograms[NumShadowProgram];
}


void InitShadow()
{
	glGenFramebuffers(NumShadowFrameBuffer, shadowFrameBuffers);

	glGenBuffers(NumShadowTextureBuffer, shadowBuffers);
	glBindTexture(GL_TEXTURE_2D, shadowBuffers[ShadowBuffer]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffers[ShadowFrameBuffer]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowBuffers[ShadowBuffer], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowPrograms[PlaneProgram] = glCreateProgram();
	ShaderInfo shadersPlane[] = {
	{GL_VERTEX_SHADER,"GBufferTexture.vert"} ,
	{GL_FRAGMENT_SHADER,"Shadow.frag"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersPlane[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersPlane[i]);
		glAttachShader(shadowPrograms[PlaneProgram], shader);
	}
	glLinkProgram(shadowPrograms[PlaneProgram]);

	shadowPrograms[GrassProgram] = glCreateProgram();
	ShaderInfo shadersGrass[] = {
	{GL_VERTEX_SHADER,"GBufferTexture.vert"} ,
	{GL_TESS_CONTROL_SHADER,"GrassControl.tesc"},
	{GL_TESS_EVALUATION_SHADER,"GrassEvaluation.tese"},
	{GL_GEOMETRY_SHADER,"GrassNormal.geom"} ,
	{GL_FRAGMENT_SHADER,"GrassShadow.frag"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersGrass[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersGrass[i]);
		glAttachShader(shadowPrograms[GrassProgram], shader);
	}
	glLinkProgram(shadowPrograms[GrassProgram]);

	shadowPrograms[WaterProgram] = glCreateProgram();
	ShaderInfo shadersWater[] = {
	{GL_VERTEX_SHADER,"GBufferWater.vert"} ,
	{GL_FRAGMENT_SHADER,"Shadow.frag"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersWater[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersWater[i]);
		glAttachShader(shadowPrograms[WaterProgram], shader);
	}
	glLinkProgram(shadowPrograms[WaterProgram]);

	shadowPrograms[DisplayShadowProgram] = glCreateProgram();
	ShaderInfo shadersDisplay[] = {
	{GL_VERTEX_SHADER,"GBufferShowShadow.vert"} ,
	{GL_FRAGMENT_SHADER,"GBufferShowShadow.frag"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersDisplay[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersDisplay[i]);
		glAttachShader(shadowPrograms[DisplayShadowProgram], shader);
	}
	glLinkProgram(shadowPrograms[DisplayShadowProgram]);

	shadowPrograms[FitPlaneShadowProgram] = glCreateProgram();
	ShaderInfo shadersFitPlane[] = {
	{GL_VERTEX_SHADER,"GBufferTexture.vert"} ,
			{GL_TESS_CONTROL_SHADER,"Fit.tesc"},
		{GL_TESS_EVALUATION_SHADER,"Fit.tese"},
		{GL_GEOMETRY_SHADER,"Fit.geom"},
	{GL_FRAGMENT_SHADER,"GrassShadow.frag"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersFitPlane[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersFitPlane[i]);
		glAttachShader(shadowPrograms[FitPlaneShadowProgram], shader);
	}
	glLinkProgram(shadowPrograms[FitPlaneShadowProgram]);
}
