#pragma once
enum { ShadowBuffer, NumShadowTextureBuffer};
enum { ShadowFrameBuffer , NumShadowFrameBuffer};
enum { PlaneProgram, WaterProgram, GrassProgram, DisplayShadowProgram, NumShadowProgram };
GLuint shadowBuffers[NumShadowTextureBuffer];
GLuint shadowFrameBuffers[NumShadowFrameBuffer];
GLuint shadowPrograms[NumShadowProgram];
const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;

void initShadow() {
	glGenFramebuffers(NumShadowFrameBuffer, shadowFrameBuffers);

	glGenBuffers(NumShadowTextureBuffer,shadowBuffers);
	glBindTexture(GL_TEXTURE_2D, shadowBuffers[ShadowBuffer]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffers[ShadowFrameBuffer]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowBuffers[ShadowBuffer], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowPrograms[PlaneProgram] = glCreateProgram();
	ShaderInfo shadersPlane[] = {
	{GL_VERTEX_SHADER,"gBufferTexturePlaneV.shader"} ,
	//{GL_GEOMETRY_SHADER,"NormalG.shader"} ,
	{GL_FRAGMENT_SHADER,"shadowF.shader"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersPlane[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersPlane[i]);
		glAttachShader(shadowPrograms[PlaneProgram], shader);
	}
	glLinkProgram(shadowPrograms[PlaneProgram]);

	shadowPrograms[GrassProgram] = glCreateProgram();
	ShaderInfo shadersGrass[] = {
	{GL_VERTEX_SHADER,"gBufferTexturePlaneV.shader"} ,
	{GL_TESS_CONTROL_SHADER,"ControlT.shader"},
	{GL_TESS_EVALUATION_SHADER,"EvaluationT.shader"},
	{GL_GEOMETRY_SHADER,"NormalG.shader"} ,
	{GL_FRAGMENT_SHADER,"shadowGF.shader"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersGrass[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersGrass[i]);
		glAttachShader(shadowPrograms[GrassProgram], shader);
	}
	glLinkProgram(shadowPrograms[GrassProgram]);

	shadowPrograms[WaterProgram] = glCreateProgram();
	ShaderInfo shadersWater[] = {
	{GL_VERTEX_SHADER,"gBufferWaterV.shader"} ,
	{GL_FRAGMENT_SHADER,"shadowF.shader"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersWater[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersWater[i]);
		glAttachShader(shadowPrograms[WaterProgram], shader);
	}
	glLinkProgram(shadowPrograms[WaterProgram]);

	shadowPrograms[DisplayShadowProgram] = glCreateProgram();
	ShaderInfo shadersDisplay[] = {
	{GL_VERTEX_SHADER,"gBufferShowShadowV.shader"} ,
	{GL_FRAGMENT_SHADER,"gBufferShowShadowF.shader"} ,
	{GL_NONE,""} };
	for (int i = 0; shadersDisplay[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersDisplay[i]);
		glAttachShader(shadowPrograms[DisplayShadowProgram], shader);
	}
	glLinkProgram(shadowPrograms[DisplayShadowProgram]);
}