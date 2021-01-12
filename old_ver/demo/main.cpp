#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "toolkit.h"
#include "Camera.h"
#include "Shadow.h"

#include "SkyBox.h"
#include "Water.h"
#include "Light.h"
#include "TexturedPlane.h"
#include "FitTexturePlane.h"
#include "TransparentPlane.h"
#include "Sphere.h"

#include "Sence.h"
#include "Interaction.h"

void init();
void prepare();
void renderShadow();
void renderGBuffer();
void renderOITBufferTM();
void renderBlendOIT();

void renderGBufferLight();
void renderLightModelGBuffer();

float waterTime;

int main(int argc, char** argv) {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "CG Project", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	init();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);

		waterTime = glfwGetTime() * 10;
		testFitPlane.updateHeight(ObjPos, ObjVel, radius, 0, camera.GetViewMatrix(), projection);
		renderShadow();
		renderGBuffer();
		renderOITBufferTM();
		renderBlendOIT();
		renderGBufferLight();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
GLuint gBufferShowProgram;
GLuint gBufferLightProgram;
void init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	initOIT(); 
	initSence();
	initShadow();
	initGBuffer();

	gBufferShowProgram = glCreateProgram();
	ShaderInfo shadersGshow[] = {
        {GL_VERTEX_SHADER,"gBufferShowV.shader"} ,
        {GL_FRAGMENT_SHADER,"gBufferShowF.shader"} ,
        {GL_NONE,""} };
	for (int i = 0; shadersGshow[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersGshow[i]);
		glAttachShader(gBufferShowProgram, shader);
	}
	glLinkProgram(gBufferShowProgram);

	gBufferLightProgram = glCreateProgram();
	ShaderInfo shadersLight[] = {
		{GL_VERTEX_SHADER,"gBufferLightV.shader"} ,
		{GL_FRAGMENT_SHADER,"gBufferLightF.shader"} ,
		{GL_NONE,""} };
	for (int i = 0; shadersLight[i].mode != GL_NONE; i++) {
		GLuint shader = LoadShader(shadersLight[i]);
		glAttachShader(gBufferLightProgram, shader);
	}
	glLinkProgram(gBufferLightProgram);
}

void prepare() {
	for (int i = 0; i < 3; i++) plane[i].prepare(directionalLight);
	//spotLight.prepare();
}

void renderGBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffers[GBuffer]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	//skybox.gBufferRender(glm::mat4(glm::mat3(camera.GetViewMatrix())), glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 500.0f));
	skybox.gBufferRender(glm::mat4(glm::mat3(camera.GetViewMatrix())), projection);
	glDepthMask(GL_TRUE);
	plane[3].grassRender(camera.GetViewMatrix(), projection, glfwGetTime(), camera.GetViewPosition());
	//for (int i = 0; i < 3; i++) plane[i].grassRender(camera.GetViewMatrix(), projection);
	for (int i = 0; i < 4; i++) 
		plane[i].gBufferRender(camera.GetViewMatrix(), projection);
	testFitPlane.gBufferRender(camera.GetViewMatrix(), projection);
	//waterPlane.gBufferRender(camera.GetViewMatrix(), projection, glfwGetTime() * 10);
}
void renderOITBufferTM() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glClearBufferfv(GL_COLOR, 0, bgColor); 
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, OITBuffers[HeadPointerInit]);
	glBindTexture(GL_TEXTURE_2D, OITTextures[HeadPointerTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	// Bind head-pointer image for read-write
	glBindImageTexture(0, OITTextures[HeadPointerTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	// Bind linked-list buffer for write
	glBindImageTexture(1, OITTextures[StorageTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32UI);
	glBindImageTexture(2, OITTextures[StorageWorldPosTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindImageTexture(3, OITTextures[StorageNormalTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, OITBuffers[AtomicCounter]);
	const GLuint zero = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);


	waterPlane.oitRender(OITPrograms[WaterOIT], camera.GetViewMatrix(), projection, waterTime);
	for (auto & plane : testTransPlane)
		plane.oitRender(OITPrograms[PlaneOIT], camera.GetViewMatrix(), projection);

	glEnable(GL_DEPTH_TEST);

}
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderBlendOIT() {
	//TODO : blendRender
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffers[GBuffer]);
	glUseProgram(OITPrograms[RankOIT]);
	GLuint location;
	location = glGetUniformLocation(OITPrograms[RankOIT], "gPosition");
	glUniform1i(location, GPositionTexture);
	location = glGetUniformLocation(OITPrograms[RankOIT], "gNormal");
	glUniform1i(location, GNormalTexture);
	location = glGetUniformLocation(OITPrograms[RankOIT], "gAlbedoSpec");
	glUniform1i(location, GAlbedoSpecTexture);
	location = glGetUniformLocation(OITPrograms[RankOIT], "gDepthID");
	glUniform1i(location, GDepthIDTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);


	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
void displayGBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(gBufferShowProgram);
	GLuint location;
	location = glGetUniformLocation(gBufferShowProgram, "gPosition");
	glUniform1i(location, GPositionTexture);
	location = glGetUniformLocation(gBufferShowProgram, "gNormal");
	glUniform1i(location, GNormalTexture);
	location = glGetUniformLocation(gBufferShowProgram, "gAlbedoSpec");
	glUniform1i(location, GAlbedoSpecTexture);
	location = glGetUniformLocation(gBufferShowProgram, "gDepthID");
	glUniform1i(location, GDepthIDTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);

	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void renderGBufferLight() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (quadVAO == 0) {
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	glUseProgram(gBufferLightProgram);
	GLuint location;
	location = glGetUniformLocation(gBufferLightProgram, "gPosition");
	glUniform1i(location, GPositionTexture);
	location = glGetUniformLocation(gBufferLightProgram, "gNormal");
	glUniform1i(location, GNormalTexture);
	location = glGetUniformLocation(gBufferLightProgram, "gAlbedoSpec");
	glUniform1i(location, GAlbedoSpecTexture);
	location = glGetUniformLocation(gBufferLightProgram, "gDepthID");
	glUniform1i(location, GDepthIDTexture);
	location = glGetUniformLocation(gBufferLightProgram, "shadowMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(gBufferLightProgram, "lightSpaceMatrix");
	glm::mat4 temp = LightProjection * LightView;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(temp));
	location = glGetUniformLocation(gBufferLightProgram, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
	location = glGetUniformLocation(gBufferLightProgram, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, shadowBuffers[ShadowBuffer]);
	SendAllLightUniformsToShader(gBufferLightProgram, directionalLight);


	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);



	glUseProgram(0);
	glBindVertexArray(0);
}

void renderLightModelGBuffer() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GBuffers[GBuffer]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//spotLight.render(camera.GetViewMatrix(), projection);
}

void renderShadow() {
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffers[ShadowFrameBuffer]);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	plane[3].shadowGrassBufferRender(shadowPrograms[GrassProgram], LightView, LightProjection, glfwGetTime(),camera.GetViewPosition());
	for (int i = 0; i < 4; i++) plane[i].shadowBufferRender(shadowPrograms[PlaneProgram], LightView, LightProjection);
	waterPlane.shadowBufferRender(shadowPrograms[WaterProgram], LightView, LightProjection, waterTime);
	testFitPlane.shadowBufferRender(shadowPrograms[FitPlaneShadowProgram], LightView, LightProjection);
	//for (int i = 0; i < 3; i++) plane[i].shadowBufferRender(shadowPrograms[PlaneProgram], camera.GetViewMatrix(), projection);
	//waterPlane.shadowBufferRender(shadowPrograms[WaterProgram], camera.GetViewMatrix(), projection, glfwGetTime() * 10);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glViewport(0, 0, width, height);
}
