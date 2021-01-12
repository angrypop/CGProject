#include "TexturedPlane.h"
#include "Shadow.h"

void TexturedPlane::RenderShadowGrassBuffer()
{
	glm::mat4 lightProjection = GlobalDataPool::GetData<glm::mat4>("lightProjection");
	glm::mat4 lightView = GlobalDataPool::GetData<glm::mat4>("lightView");
	glm::vec3 uniObjPos = GlobalDataPool::GetData<glm::vec3>("cameraPosition");
	GLfloat time = GlobalDataPool::GetData<GLfloat>("systemTime");

	GLint location;
	GLuint program = Shadow::shadowPrograms[Shadow::GrassProgram];
	glUseProgram(program);

	location = glGetUniformLocation(program, "uniM");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	location = glGetUniformLocation(program, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightView));
	location = glGetUniformLocation(program, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightProjection));
	location = glGetUniformLocation(program, "uniTime");
	glUniform1f(location, time);
	location = glGetUniformLocation(program, "uniObjPos");
	glUniform3fv(location, 1, glm::value_ptr(uniObjPos));
	glBindVertexArray(this->VAOs[PlaneVAO]);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4);
}

void TexturedPlane::RenderShadowBuffer()
{
	glm::mat4 lightProjection = GlobalDataPool::GetData<glm::mat4>("lightProjection");
	glm::mat4 lightView = GlobalDataPool::GetData<glm::mat4>("lightView");
	if (renderGrassFlag)
	{
		this->RenderShadowGrassBuffer();
	}
	GLint location;
	GLuint program = Shadow::shadowPrograms[Shadow::PlaneProgram];
	glUseProgram(program);

	location = glGetUniformLocation(program, "uniM");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	location = glGetUniformLocation(program, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightView));
	location = glGetUniformLocation(program, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightProjection));

	glBindVertexArray(this->VAOs[PlaneVAO]);
	glDrawElements(GL_TRIANGLES, this->VertexNum, GL_UNSIGNED_INT, 0);
}
