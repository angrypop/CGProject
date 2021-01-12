#include "TransparentPlane.h"
#include "Scene.h"

void TransparentPlane::RenderOITBuffer()
{
	GLuint program = OITPrograms[PlaneOIT];
	glm::mat4 uniP = GlobalDataPool::GetData<glm::mat4>("cameraProjection");
	glm::mat4 uniV = GlobalDataPool::GetData<glm::mat4>("cameraView");
	glUseProgram(program);

	GLint location;
	location = glGetUniformLocation(program, "uniM");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
	location = glGetUniformLocation(program, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
	location = glGetUniformLocation(program, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
	location = glGetUniformLocation(program, "planeColor");
	glUniform4fv(location, 1, glm::value_ptr(color));
	glBindVertexArray(this->VAOs[PlaneVAO]);
	glDrawElements(GL_TRIANGLES, this->VertexNum, GL_UNSIGNED_INT, 0);
}
