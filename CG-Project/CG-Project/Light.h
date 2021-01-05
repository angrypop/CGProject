#pragma once
#include "ViewBase.h"

enum { LightVAO, NumLightVAO };
enum { LightArrayBuffer, LightElementBuffer, NumLightBuffer };

class Light {
public:
	glm::vec3 LightPos;
	glm::vec3 LightColor;

	glm::mat4 modelMat;

	GLuint VAOs[NumLightVAO];
	GLuint Buffers[NumLightBuffer];
	GLuint Program;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	void translate(glm::mat4 translate) { this->modelMat = translate * modelMat; }
	void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
	void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	void setSpecular(glm::vec3 specular) { this->specular = specular; }

	virtual void prepare() {}
	virtual void render(glm::mat4 V, glm::mat4 P, glm::mat4 translate = glm::mat4(1)) {}
};

class DirectionalLight : public Light {
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