#pragma once
#include "Shadow.h"

const int width = 1280;
const int height = 768;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 500.0f);

GLfloat groundSize = 15.0f;
GLfloat lightSize = 0.5f;
GLfloat ambientStrength = 0.1f;
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition(0.0f, 20.0f, 0.0f);
//light scene
SpotLight spotLight;
DirectionalLight directionalLight;

TexturedPlane plane[4];
WaterPlane waterPlane;
WaterPlane waterPlane2;
Camera camera(glm::vec3(40.0f, 40.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 LightProjection;
glm::mat4 LightView;

enum { GBuffer, NumBuffer };
enum { GPositionTexture, GNormalTexture, GAlbedoSpecTexture, GDepthIDTexture, NumGTexture };
GLuint GBuffers[NumBuffer];
GLuint GTextures[NumGTexture];

const int StroageSize = 2048 * 2048;
enum { HeadPointerInit, Storage, StorageWorldPos, StorageNormal, AtomicCounter, NumOITBuffer };
enum { HeadPointerTexture, StorageTexture, StorageWorldPosTexture, StorageNormalTexture, NumOITTexture };
enum { WaterOIT, RankOIT, DisplayOIT, NumOITProgram };
GLuint OITBuffers[NumOITBuffer];
GLuint OITTextures[NumOITTexture];
GLuint OITPrograms[NumOITProgram];

void initScene() {
	//plane set
	GLfloat planePoints1[] = {
		-groundSize , 0.0f , -groundSize , 0.0f , 0.0f ,
		 groundSize , 0.0f , -groundSize , 1.0f , 0.0f ,
		-groundSize , 0.0f ,  groundSize , 0.0f , 1.0f ,
		 groundSize , 0.0f ,  groundSize , 1.0f , 1.0f };
	GLuint planeIndices1[] = {
		 0, 1, 2, // first triangle
		 1, 2, 3  // second triangle
	};
	ShaderInfo planeShaders[] = {
		{GL_VERTEX_SHADER,"GroundV.shader"} ,
		{GL_FRAGMENT_SHADER,"GroundF.shader"} ,
		{GL_NONE,""} };
	TextureInfo planeTextures[] = {
		{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Color.jpg"},
		{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
		{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
		{Texture_NONE,""} };
	TextureInfo grassTextures[] = {
		{AmbientTexture,"Ground003_4K-JPG/Ground003_4K_Color.jpg"},
		{NormalTexture,"Ground003_4K-JPG/Ground003_4K_Normal.jpg"},
		{RoughnessTexture,"Ground003_4K-JPG/Ground003_4K_Roughness.jpg"},
		{Texture_NONE,""} };
	GLfloat planePoints2[] = {
	-groundSize , groundSize * 2 , -groundSize , 0.0f , 0.0f ,
	 groundSize , groundSize * 2 , -groundSize , 1.0f , 0.0f ,
	-groundSize , 0 , -groundSize , 0.0f , 1.0f ,
	 groundSize , 0 , -groundSize , 1.0f , 1.0f };
	GLuint planeIndices2[] = {
		 0, 1, 2, // first triangle
		 1, 2, 3  // second triangle
	};
	GLfloat planePoints3[] = {
	 -groundSize , 0 , -groundSize , 0.0f , 0.0f ,
	 -groundSize , 2 * groundSize , -groundSize , 0.0f , 1.0f ,
	 -groundSize , 0 ,  groundSize , 1.0f , 0.0f ,
	 -groundSize , 2 * groundSize ,  groundSize , 1.0f , 1.0f };
	GLuint planeIndices3[] = {
		 0, 1, 2, // first triangle
		 1, 2, 3  // second triangle
	};
	GLfloat planePoints4[] = {
	groundSize , groundSize * 3 / 2 , groundSize , 0.0f , 0.0f ,
	groundSize , groundSize * 3 / 2 , groundSize * 3 , 0.0f , 1.0f ,
	groundSize * 3, groundSize * 3 / 2 , groundSize , 1.0f, 0.0f ,
	groundSize * 3 , groundSize * 3 / 2 ,  groundSize * 3 , 1.0f , 1.0f };
	plane[0] = TexturedPlane(glm::vec3(0, 0, 0), planePoints1, sizeof(planePoints1), planeIndices1, sizeof(planeIndices1), planeShaders, planeTextures);
	plane[1] = TexturedPlane(glm::vec3(0, 0, 0), planePoints2, sizeof(planePoints2), planeIndices2, sizeof(planeIndices2), planeShaders, planeTextures);
	plane[2] = TexturedPlane(glm::vec3(0, 0, 0), planePoints3, sizeof(planePoints3), planeIndices3, sizeof(planeIndices3), planeShaders, planeTextures);
	plane[3] = TexturedPlane(glm::vec3(0, 0, 0), planePoints4, sizeof(planePoints4), planeIndices3, sizeof(planeIndices3), planeShaders, grassTextures);

	//light
	GLfloat lightPoints[] = {
		lightSize , lightSize, lightSize,
		lightSize , lightSize, -lightSize,
		lightSize , -lightSize, lightSize,
		lightSize , -lightSize, -lightSize,
		-lightSize , lightSize, lightSize,
		-lightSize , lightSize, -lightSize,
		-lightSize , -lightSize, lightSize,
		-lightSize , -lightSize, -lightSize };
	GLuint lightIndices[] = {
		0,1,4,	1,4,5,
		0,1,2,  1,2,3,
		4,5,6,  5,6,7,
		2,3,6,  3,6,7,
		0,2,4,  2,4,6 };
	ShaderInfo shadersLight[] = {
	{GL_VERTEX_SHADER,"LightV.shader"} ,
	{GL_FRAGMENT_SHADER,"LightF.shader"} ,
	{GL_NONE,""} };
	spotLight = SpotLight(lightPosition, lightPoints, sizeof(lightPoints), lightIndices, sizeof(lightIndices), shadersLight);

	//dir light
	directionalLight = DirectionalLight(glm::vec3(-1.0, 1.0, -1.0));
	//LightProjection = glm::ortho(0.0f, SHADOW_WIDTH * 1.0f, 0.0f, SHADOW_HEIGHT * 1.0f, 0.1f, 1000.0f);
	float e = 0.03;
	LightProjection = glm::ortho(SHADOW_WIDTH * -0.5f * e, SHADOW_WIDTH * 0.5f * e, SHADOW_HEIGHT * -0.5f * e, SHADOW_HEIGHT * 0.5f * e, 0.1f, 1000.0f);
	//LightProjection = glm::ortho(-0.5f * width, width * 0.5f, -0.5f * height, 0.5f * height, 0.1f, 2000.0f);
	//LightProjection = glm::ortho(0.f, width * 1.0f, 0.f, 1.f * height, 0.1f, 2000.0f);
	LightView = glm::lookAt(directionalLight.LightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//water 
	waterPlane = WaterPlane(-groundSize * 5, -groundSize * 5, groundSize * 5, groundSize * 5, groundSize, 100, 100);
	//waterPlane2 = WaterPlane(-groundSize * 5, -groundSize * 5, groundSize * 5, groundSize * 5, 0, 100, 100);
}

void initGBuffer() {
	glGenFramebuffers(NumBuffer, GBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffers[GBuffer]);

	glGenTextures(NumGTexture, GTextures);
	// - 位置颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GTextures[GPositionTexture], 0);

	// - 法线颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GTextures[GNormalTexture], 0);

	// - 颜色 + 镜面颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture], 0);

	// - 深度缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, GTextures[GDepthIDTexture], 0);

	// - 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << " " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void initOIT() {
	GLuint* data;

	glGenBuffers(NumOITBuffer, OITBuffers);
	glGenTextures(NumOITTexture, OITTextures);

	glBindTexture(GL_TEXTURE_2D, OITTextures[HeadPointerTexture]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindImageTexture(0, OITTextures[HeadPointerTexture], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, OITBuffers[HeadPointerInit]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	data = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	memset(data, 0x00, width * height * sizeof(GLuint));
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, OITBuffers[AtomicCounter]);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

	glBindBuffer(GL_TEXTURE_BUFFER, OITBuffers[Storage]);
	glBufferData(GL_TEXTURE_BUFFER, 2 * width * height * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glBindTexture(GL_TEXTURE_BUFFER, OITTextures[StorageTexture]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, OITTextures[StorageTexture]);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	glBindBuffer(GL_TEXTURE_BUFFER, OITBuffers[StorageWorldPos]);
	glBufferData(GL_TEXTURE_BUFFER, 2 * width * height * sizeof(glm::vec3), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glBindTexture(GL_TEXTURE_BUFFER, OITTextures[StorageWorldPosTexture]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, OITTextures[StorageWorldPosTexture]);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	glBindBuffer(GL_TEXTURE_BUFFER, OITBuffers[StorageNormal]);
	glBufferData(GL_TEXTURE_BUFFER, 2 * width * height * sizeof(glm::vec3), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glBindTexture(GL_TEXTURE_BUFFER, OITTextures[StorageNormalTexture]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, OITTextures[StorageNormalTexture]);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	OITPrograms[WaterOIT] = glCreateProgram();
	ShaderInfo waterOITShaders[] = {
	//{GL_VERTEX_SHADER,"WaterOITV.shader"} ,
		{GL_VERTEX_SHADER,"gBufferWaterV.shader"} ,
	{GL_FRAGMENT_SHADER,"WaterOITF.shader"} ,
	{GL_NONE,""} };
	for (int i = 0; waterOITShaders[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(waterOITShaders[i]);
		glAttachShader(OITPrograms[WaterOIT], shader);
	}
	glLinkProgram(OITPrograms[WaterOIT]);

	OITPrograms[RankOIT] = glCreateProgram();
	ShaderInfo rankOITShaders[] = {
			{GL_VERTEX_SHADER,"RankOITV.shader"} ,
		{GL_FRAGMENT_SHADER,"RankOITF.shader"} ,
		{GL_NONE,""} };
	for (int i = 0; rankOITShaders[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(rankOITShaders[i]);
		glAttachShader(OITPrograms[RankOIT], shader);
	}
	glLinkProgram(OITPrograms[RankOIT]);

	OITPrograms[DisplayOIT] = glCreateProgram();
	ShaderInfo displayOITShaders[] = {
			{GL_VERTEX_SHADER,"DisplayOITV.shader"} ,
		{GL_FRAGMENT_SHADER,"DisplayOITF.shader"} ,
		{GL_NONE,""} };
	for (int i = 0; displayOITShaders[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(displayOITShaders[i]);
		glAttachShader(OITPrograms[DisplayOIT], shader);
	}
	glLinkProgram(OITPrograms[DisplayOIT]);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}

void SendAllLightUniformsToShader(GLuint program,Light light) {
	GLuint location;

	location = glGetUniformLocation(program, "lightColor");
	glUniform3fv(location, 1, glm::value_ptr(light.LightColor));
	location = glGetUniformLocation(program, "light.ambient");
	glUniform3fv(location, 1, glm::value_ptr(light.ambient));
	location = glGetUniformLocation(program, "light.diffuse");
	glUniform3fv(location, 1, glm::value_ptr(light.diffuse));
	location = glGetUniformLocation(program, "light.specular");
	glUniform3fv(location, 1, glm::value_ptr(light.specular));
	location = glGetUniformLocation(program, "light.constant");
	glUniform1f(location, light.constant);
	location = glGetUniformLocation(program, "light.linear");
	glUniform1f(location, light.linear);
	location = glGetUniformLocation(program, "light.quadratic");
	glUniform1f(location, light.quadratic);
	location = glGetUniformLocation(program, "viewPos");
	glUniform3fv(location, 1, glm::value_ptr(camera.GetViewPosition()));
	location = glGetUniformLocation(program, "light.position");
	glUniform3fv(location, 1, glm::value_ptr(light.LightPos));
}