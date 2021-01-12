#include "ViewObject.h"

void ViewObject::Rotate(const GLfloat& angle, const glm::vec3& axis)
{
	this->modelMat = glm::rotate(this->modelMat, glm::radians(angle), axis);
}

void ViewObject::Translate(const glm::vec3& displacement)
{
	this->modelMat = glm::translate(this->modelMat, displacement);
}

void ViewObject::Scale(const glm::vec3& scaler)
{
	this->modelMat = glm::scale(this->modelMat, scaler);
}

glm::mat4 ViewObject::GetM() const
{
	return this->modelMat;
}

void ViewObject::SetM(const glm::mat4& M)
{
	this->modelMat = M;
}
