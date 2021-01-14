#include "FitTexturePlane.h"

void FitTexturedPlane::RenderShadowBuffer()
{
	glm::mat4 lightProjection = GlobalDataPool::GetData<glm::mat4>("lightProjection");
	glm::mat4 lightView = GlobalDataPool::GetData<glm::mat4>("lightView");
	GLint location;
	GLuint program = Shadow::shadowPrograms[Shadow::FitPlaneShadowProgram];
	glUseProgram(program);

	location = glGetUniformLocation(program, "uniM");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	location = glGetUniformLocation(program, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightView));
	location = glGetUniformLocation(program, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(lightProjection));
	location = glGetUniformLocation(program, "heightMap");
	glUniform1i(location, FitHeightTexture);

	GLfloat uniNear = GlobalDataPool::GetData<GLfloat>("uniNear");
	location = glGetUniformLocation(program, "uniNear");
	glUniform1f(location, uniNear);
	GLfloat uniFar = GlobalDataPool::GetData<GLfloat>("uniFar");
	location = glGetUniformLocation(program, "uniFar");
	glUniform1f(location, uniFar);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->Textures[FitHeightTexture]);

	glBindVertexArray(this->VAOs[FitPlaneVAO]);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4 * part * part);
}
