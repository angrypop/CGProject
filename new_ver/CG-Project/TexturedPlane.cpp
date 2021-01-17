#include "TexturedPlane.h"
#include "Shadow.h"

void TexturedPlane::ChangeTexture(std::vector<TextureInfo> infos)
{
	for (const auto & info : infos) 
	{
		this->hasTextures[info.mode] = 1;
		this->Textures[info.mode] = LoadTexture(info);
	}
}

void TexturedPlane::RenderGrassGBuffer()
{
	glm::mat4 cameraView = GlobalDataPool::GetData<glm::mat4>("cameraView");
	GLfloat uniTime = GlobalDataPool::GetData<GLfloat>("systemTime");
	glm::vec3 ObjPos = GlobalDataPool::GetData<glm::vec3>("cameraPosition");
	glm::mat4 cameraProjection = GlobalDataPool::GetData<glm::mat4>("cameraProjection");
	GLint location;
	glUseProgram(GrassProgram);

	location = glGetUniformLocation(GrassProgram, "uniM");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	location = glGetUniformLocation(GrassProgram, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraView));
	location = glGetUniformLocation(GrassProgram, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraProjection));
	location = glGetUniformLocation(GrassProgram, "uniTime");
	glUniform1f(location, uniTime);
	location = glGetUniformLocation(GrassProgram, "uniObjPos");
	glUniform3fv(location, 1, glm::value_ptr(ObjPos));

	GLfloat uniNear = GlobalDataPool::GetData<GLfloat>("uniNear");
	location = glGetUniformLocation(GrassProgram, "uniNear");
	glUniform1f(location, uniNear);
	GLfloat uniFar = GlobalDataPool::GetData<GLfloat>("uniFar");
	location = glGetUniformLocation(GrassProgram, "uniFar");
	glUniform1f(location, uniFar);


	glBindVertexArray(this->VAOs[PlaneVAO]);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4);
	glUseProgram(0);
}

void TexturedPlane::RenderGBuffer()
{
	if (this->renderGrassFlag)
	{
		this->RenderGrassGBuffer();
	}
	glm::mat4 uniP = GlobalDataPool::GetData<glm::mat4>("cameraProjection");
	glm::mat4 uniV = GlobalDataPool::GetData<glm::mat4>("cameraView");
	GLint location;
	glUseProgram(GBufferProgram);

	location = glGetUniformLocation(GBufferProgram, "uniM");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	location = glGetUniformLocation(GBufferProgram, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
	location = glGetUniformLocation(GBufferProgram, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
	location = glGetUniformLocation(GBufferProgram, "TBN");
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(this->TBN));
	location = glGetUniformLocation(GBufferProgram, "colorTexture");
	glUniform1i(location, PlaneColorTexture);
	location = glGetUniformLocation(GBufferProgram, "normalTexture");
	glUniform1i(location, PlaneNormalTexture);
	location = glGetUniformLocation(GBufferProgram, "roughnessTexture");
	glUniform1i(location, PlaneRoughnessTexture);
	GLfloat uniNear = GlobalDataPool::GetData<GLfloat>("uniNear");
	location = glGetUniformLocation(GBufferProgram, "uniNear");
	glUniform1f(location, uniNear);
	GLfloat uniFar = GlobalDataPool::GetData<GLfloat>("uniFar");
	location = glGetUniformLocation(GBufferProgram, "uniFar");
	glUniform1f(location, uniFar);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->Textures[PlaneColorTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->Textures[PlaneNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->Textures[PlaneRoughnessTexture]);


	glBindVertexArray(this->VAOs[PlaneVAO]);
	glDrawElements(GL_TRIANGLES, this->VertexNum, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
}

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

	GLfloat uniNear = GlobalDataPool::GetData<GLfloat>("uniNear");
	location = glGetUniformLocation(program, "uniNear");
	glUniform1f(location, uniNear);
	GLfloat uniFar = GlobalDataPool::GetData<GLfloat>("uniFar");
	location = glGetUniformLocation(program, "uniFar");
	glUniform1f(location, uniFar);

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

	GLfloat uniNear = GlobalDataPool::GetData<GLfloat>("uniNear");
	location = glGetUniformLocation(program, "uniNear");
	glUniform1f(location, uniNear);
	GLfloat uniFar = GlobalDataPool::GetData<GLfloat>("uniFar");
	location = glGetUniformLocation(program, "uniFar");
	glUniform1f(location, uniFar);

	glBindVertexArray(this->VAOs[PlaneVAO]);
	glDrawElements(GL_TRIANGLES, this->VertexNum, GL_UNSIGNED_INT, 0);
}

void TexturedPlane::prepare(Light& light)
{
	GLuint location;

	glUseProgram(Program);
	//location = glGetUniformLocation(Program, "ambientStrength");
	//glUniform1f(location, ambientStrength);
	location = glGetUniformLocation(Program, "lightColor");
	glUniform3fv(location, 1, glm::value_ptr(light.LightColor));
	location = glGetUniformLocation(Program, "colorTexture");
	glUniform1i(location, PlaneColorTexture);
	location = glGetUniformLocation(Program, "normalTexture");
	glUniform1i(location, PlaneNormalTexture);
	location = glGetUniformLocation(Program, "roughnessTexture");
	glUniform1i(location, PlaneRoughnessTexture);
	location = glGetUniformLocation(Program, "light.ambient");
	glUniform3fv(location, 1, glm::value_ptr(light.ambient));
	location = glGetUniformLocation(Program, "light.diffuse");
	glUniform3fv(location, 1, glm::value_ptr(light.diffuse));
	location = glGetUniformLocation(Program, "light.specular");
	glUniform3fv(location, 1, glm::value_ptr(light.specular));
	location = glGetUniformLocation(Program, "light.constant");
	glUniform1f(location, light.constant);
	location = glGetUniformLocation(Program, "light.linear");
	glUniform1f(location, light.linear);
	location = glGetUniformLocation(Program, "light.quadratic");
	glUniform1f(location, light.quadratic);
	location = glGetUniformLocation(Program, "TBN");
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(this->TBN));
}
