#include "Water.h"
#include "Scene.h"

void WaterPlane::RenderShadowBuffer()
{
	GLfloat waterTime = GlobalDataPool::GetData<GLfloat>("systemTime") * 10;
	glm::mat4 lightProjection = GlobalDataPool::GetData<glm::mat4>("lightProjection");
	glm::mat4 lightView = GlobalDataPool::GetData<glm::mat4>("lightView");
	GLint location;
	GLuint program = Shadow::shadowPrograms[Shadow::WaterProgram];
	glUseProgram(program);

	location = glGetUniformLocation(program, "uniM");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	location = glGetUniformLocation(program, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightView));
	location = glGetUniformLocation(program, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightProjection));
	location = glGetUniformLocation(program, "uniTime");
	glUniform1f(location, waterTime);
	glBindVertexArray(this->VAOs[WaterVAO]);
	glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

void WaterPlane::RenderOITBuffer()
{
	GLuint program = OITPrograms[WaterOIT];
	glm::mat4 uniV = GlobalDataPool::GetData<glm::mat4>("cameraView");
	GLfloat waterTime = GlobalDataPool::GetData<GLfloat>("systemTime") * 10;
	glm::mat4 uniP = GlobalDataPool::GetData<glm::mat4>("cameraProjection");

	glUseProgram(program);

	GLint location;
	location = glGetUniformLocation(program, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
	location = glGetUniformLocation(program, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
	location = glGetUniformLocation(program, "uniTime");
	glUniform1f(location, waterTime);
	glBindVertexArray(this->VAOs[WaterVAO]);
	glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}
