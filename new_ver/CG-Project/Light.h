#pragma once

#include "toolkit.h"

enum { LightVAO, NumLightVAO };
enum { LightArrayBuffer, LightElementBuffer, NumLightBuffer };

class Light {
public:
	glm::vec3 LightPos = glm::vec3(1.0f);
	glm::vec3 LightColor = glm::vec3(1.0f);

	glm::mat4 modelMat = glm::mat4(1.0f);

	GLuint VAOs[NumLightVAO]{};
	GLuint Buffers[NumLightBuffer]{};
	GLuint Program = 0;

	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);

	float constant = 0.0f;
	float linear = 0.0f;
	float quadratic = 0.0f;

	void translate(glm::mat4 translate) { this->modelMat = translate * modelMat; }
	void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
	void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	void setSpecular(glm::vec3 specular) { this->specular = specular; }

	virtual void prepare() {}
	virtual void render(glm::mat4 uniV, glm::mat4 uniP, glm::mat4 translate = glm::mat4(1)) {}
};

class SpotLight : public Light {
public:
	SpotLight() {}
	SpotLight(glm::vec3 position, GLfloat points[], GLint sizeofPoints, GLuint indices[], GLint sizeofIndices, ShaderInfo shaders[]) {
		LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		LightPos = position;

		glGenVertexArrays(NumLightVAO, this->VAOs);
		glCreateBuffers(NumLightBuffer, this->Buffers);

		glNamedBufferStorage(this->Buffers[LightArrayBuffer], sizeofPoints, points, 0);
		glNamedBufferStorage(this->Buffers[LightElementBuffer], sizeofIndices, indices, 0);

		glBindVertexArray(this->VAOs[LightVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, this->Buffers[LightArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Buffers[LightElementBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);

		Program = glCreateProgram();
		for (int i = 0; shaders[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shaders[i]);
			glAttachShader(Program, shader);
		}
		glLinkProgram(Program);
		setAmbient(glm::vec3(0.6, 0.6, 0.6));
		setDiffuse(glm::vec3(0.7, 0.7, 0.7));
		setSpecular(glm::vec3(1, 1, 1));

		modelMat = glm::translate(glm::mat4(1.0), LightPos);

		constant = 1.0f;
		linear = 0.005f;
		quadratic = 0.00075f;
	}
	virtual void prepare() {}

	virtual void render(glm::mat4 uniV, glm::mat4 uniP, glm::mat4 translate = glm::mat4(1)) {
		GLint location;
		glUseProgram(Program);
		location = glGetUniformLocation(Program, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(translate * modelMat));
		location = glGetUniformLocation(Program, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(Program, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));

		glBindVertexArray(this->VAOs[LightVAO]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
};

class DirectionalLight: public Light{
public:
	DirectionalLight() {}
	DirectionalLight(glm::vec3 direction) {
		LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		LightPos = normalize(direction) * 800.f;

		setAmbient(glm::vec3(0.6, 0.6, 0.6));
		setDiffuse(glm::vec3(0.7, 0.7, 0.7));
		setSpecular(glm::vec3(1, 1, 1));

		constant = 1.0f;
		linear = 0;
		quadratic = 0;
	}
	virtual void prepare() {}

	virtual void render() {}
};