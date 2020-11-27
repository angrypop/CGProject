#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include "toolkit.h"
#include "Camera.h"
#include "Shadow.h"

#include "Water.h"
#include "Light.h"
#include "TexturedPlane.h"

#include "Scene.h"
#include "Interaction.h"

void init();
void prepare();
void display();
void renderShadow();
void renderGBuffer();
void renderOITBufferTM();
void renderBlendOIT();
void displayShadow();
void displayGBuffer();
void displayOITBuffer();
void renderGBufferLight();
void renderLightModelGBuffer();

float waterTime;

int main(int argc, char** argv) {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "texture", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	init();
	prepare();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	int start = clock();
	int end, cnt = 0, index = 0;
	int fps[5];
	for (int i = 0; i < 5; i++)
		fps[i] = 0.0;
	while (!glfwWindowShouldClose(window)) {
		//glfwPollEvents();


		//glCullFace(GL_FRONT);
		//renderGrass();
		waterTime = glfwGetTime() * 10;
		renderShadow();
		//displayShadow();
		renderGBuffer();
		//displayGBuffer();
		renderOITBufferTM();
		//displayOITBuffer();
		renderBlendOIT();
		renderGBufferLight();
		//glCullFace(GL_BACK);
		//renderLightModelGBuffer();
		//display();

		glfwSwapBuffers(window);
		glfwPollEvents();
		cnt++;
		end = clock();
		if ((double)(end - start) / CLOCKS_PER_SEC >= 0.5)
		{
			start = end;
			fps[index] = cnt;
			index = (index + 1) % 5;
			cnt = 0;
			double sum = 0.0;
			for (int i = 0; i < 5; i++)
				sum += fps[i];
			std::cout << "fps = " << sum / (0.5 * 5) << std::endl;
		}
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
	initScene();
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
	spotLight.prepare();
}

void renderGBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffers[GBuffer]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	plane[3].grassRender(camera.GetViewMatrix(), projection, glfwGetTime(), camera.GetViewPosition());
	//for (int i = 0; i < 3; i++) plane[i].grassRender(camera.GetViewMatrix(), projection);
	for (int i = 0; i < 4; i++) plane[i].gBufferRender(camera.GetViewMatrix(), projection);
	//waterPlane.gBufferRender(camera.GetViewMatrix(), projection, glfwGetTime() * 10);
}
void renderOITBufferTM() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GLfloat bgColor[4] = {
	//	//0.5f,0.5f,0.5f,1.0f 
	//	//1.0f,1.0f,1.0f,1.0f
	//	0.0f,0.0f,0.0f,1.0f
	//};
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

	//waterPlane.render(camera.GetViewMatrix(), projection, glfwGetTime() * 10);
	waterPlane.oitRender(OITPrograms[WaterOIT], camera.GetViewMatrix(), projection, waterTime);
	//waterPlane2.oitRender(OITPrograms[WaterOIT], camera.GetViewMatrix(), projection, glfwGetTime() * 10);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//waterPlane.render(camera.GetViewMatrix(), projection, glfwGetTime() * 10);
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
	//location = glGetUniformLocation(OITPrograms[RankOIT], "gDepthID");
	//glUniform1i(location, 4);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	//glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_2D, OITTextures[HeadPointerTexture]);

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

//glm::mat4 LightProjection = glm::ortho(0.0f, SHADOW_WIDTH * 1.0f, 0.0f, SHADOW_HEIGHT * 1.0f, 0.1f, 1000.0f);
//glm::mat4 LightView = glm::lookAt(directionalLight.LightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
	location = glGetUniformLocation(gBufferLightProgram, "P");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
	location = glGetUniformLocation(gBufferLightProgram, "V");
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
	//SendAllLightUniformsToShader(gBufferLightProgram, spotLight);
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

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat bgColor[4] = { 
		//0.5f,0.5f,0.5f,1.0f 
		//1.0f,1.0f,1.0f,1.0f
		0.0f,0.0f,0.0f,1.0f
	};
	glClearBufferfv(GL_COLOR, 0, bgColor);

	double time = glfwGetTime();

	glm::vec3 lightOffset = glm::vec3(glm::sin(time) * 10, 0, glm::cos(time) * 10);
	//render with spot light
	//plane.render(spotLight.LightPos + lightOffset, camera.GetViewPosition(), camera.GetViewMatrix(), projection);
	//spotLight.render(camera.GetViewMatrix(), projection, glm::translate(glm::mat4(1), lightOffset));

	//render with dir light
	for (int i = 0; i < 3; i++) plane[i].render(directionalLight.LightPos, camera.GetViewPosition(), camera.GetViewMatrix(), projection);
	directionalLight.render();
	waterPlane.render(camera.GetViewMatrix(), projection, time);

	//Coord
	glUseProgram(0);
	glBindVertexArray(0);

	glm::vec4 p1 = glm::vec4(0, 0, 0,1.0f);
	glm::vec4 p2 = glm::vec4(10, 0, 0, 1);
	glm::vec4 p3 = glm::vec4(0, 10, 0, 1);
	glm::vec4 p4 = glm::vec4(0, 0, 10, 1);
	p1 = projection * camera.GetViewMatrix() * model * p1; p1 /= p1.w;
	p2 = projection * camera.GetViewMatrix() * model * p2; p2 /= p2.w;
	p3 = projection * camera.GetViewMatrix() * model * p3; p3 /= p3.w;
	p4 = projection * camera.GetViewMatrix() * model * p4; p4 /= p4.w;
	glLineWidth(100);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p3.x, p3.y, p3.z);
	glEnd();
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p4.x, p4.y, p4.z);
	glEnd();
}

//glm::mat4 LightProjection = glm::ortho(-0.5f * width/2, width * 0.5f/2, -0.5f * height/2, 0.5f * height/2, 0.1f, 1000.0f);
void renderShadow() {
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffers[ShadowFrameBuffer]);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	plane[3].shadowGrassBufferRender(shadowPrograms[GrassProgram], LightView, LightProjection, glfwGetTime(),camera.GetViewPosition());
	for (int i = 0; i < 4; i++) plane[i].shadowBufferRender(shadowPrograms[PlaneProgram], LightView, LightProjection);
	waterPlane.shadowBufferRender(shadowPrograms[WaterProgram], LightView, LightProjection, waterTime);
	//for (int i = 0; i < 3; i++) plane[i].shadowBufferRender(shadowPrograms[PlaneProgram], camera.GetViewMatrix(), projection);
	//waterPlane.shadowBufferRender(shadowPrograms[WaterProgram], camera.GetViewMatrix(), projection, glfwGetTime() * 10);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glViewport(0, 0, width, height);
}
void displayShadow() {
	//static GLuint quadVAO = 0;
	//static GLuint quadVBO = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowPrograms[DisplayShadowProgram]);
	GLuint location;
	location = glGetUniformLocation(shadowPrograms[DisplayShadowProgram], "shadowMap");
	glUniform1i(location, ShadowBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowBuffers[ShadowBuffer]);

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

void displayCoord() {
	glm::vec4 p1 = glm::vec4(0, 0, 0, 1.0f);
	glm::vec4 p2 = glm::vec4(10, 0, 0, 1);
	glm::vec4 p3 = glm::vec4(0, 10, 0, 1);
	glm::vec4 p4 = glm::vec4(0, 0, 10, 1);
	p1 = projection * camera.GetViewMatrix() * model * p1; p1 /= p1.w;
	p2 = projection * camera.GetViewMatrix() * model * p2; p2 /= p2.w;
	p3 = projection * camera.GetViewMatrix() * model * p3; p3 /= p3.w;
	p4 = projection * camera.GetViewMatrix() * model * p4; p4 /= p4.w;
	glLineWidth(100);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p3.x, p3.y, p3.z);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p4.x, p4.y, p4.z);
	glEnd();
}

void displayOITBuffer() {
	//static int runTime = 0;
	//std::cout << runTime++;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(OITPrograms[DisplayOIT]);
	GLuint location;
	location = glGetUniformLocation(OITPrograms[DisplayOIT], "gPosition");
	glUniform1i(location, GPositionTexture);
	location = glGetUniformLocation(OITPrograms[DisplayOIT], "gNormal");
	glUniform1i(location, GNormalTexture);
	location = glGetUniformLocation(OITPrograms[DisplayOIT], "gAlbedoSpec");
	glUniform1i(location, GAlbedoSpecTexture);
	location = glGetUniformLocation(OITPrograms[DisplayOIT], "gDepthID");
	glUniform1i(location, GDepthIDTexture);
	//location = glGetUniformLocation(OITPrograms[RankOIT], "gDepthID");
	//glUniform1i(location, 4);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	//glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_2D, OITTextures[HeadPointerTexture]);

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