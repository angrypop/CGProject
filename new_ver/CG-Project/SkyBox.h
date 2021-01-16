#pragma once
#include "GlobalData.h"
#include "ViewObject.h"
#include "toolkit.h"
class SkyBox : public ViewObject
{
public:
	enum { VAO, NumVAO };
	enum { ArrayBuffer, ElementBuffer, NumBuffer };
	enum { position = 0, texCoord = 1 };

	GLuint VAOs[NumVAO];
	GLuint Buffers[NumBuffer];
	//GLuint Program;
	GLuint GBufferProgram;
	//GLuint GrassProgram;
	//GLuint updateProgram;
	GLuint cubemapTexture;

public:
	//SkyBox() {};
	SkyBox(int k);

	void RenderGBuffer();

	void RenderOITBuffer(GLuint program, glm::mat4 uniV, glm::mat4 uniP);


	//void render(glm::mat4 uniV, glm::mat4 uniP) {
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	glLineWidth(10);

	//	glUseProgram(GBufferProgram);

	//	GLint location;
	//	location = glGetUniformLocation(GBufferProgram, "uniM");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
	//	location = glGetUniformLocation(GBufferProgram, "uniV");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
	//	location = glGetUniformLocation(GBufferProgram, "uniP");
	//	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
	//	location = glGetUniformLocation(GBufferProgram, "skybox");
	//	glUniform1i(location, 0);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	//	glBindVertexArray(this->VAOs[VAO]);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//}

};