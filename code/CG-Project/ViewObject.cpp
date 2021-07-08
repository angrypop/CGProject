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

void ViewObject::Render(const ViewPassEnum& pass)
{
	if (!this->_showFlag) // hidden object
		return;

	switch (pass)
	{
	case ViewPassEnum::GBuffer:
		this->RenderGBuffer();
		break;
	case ViewPassEnum::ShadowBuffer:
		this->RenderShadowBuffer();
		break;
	case ViewPassEnum::OITBuffer:
		this->RenderOITBuffer();
		break;
	case ViewPassEnum::BlendOIT:
		this->RenderBlendOIT();
		break;
	case ViewPassEnum::GBufferIlluminate:
		this->RenderGBufferIlluminate();
		break;
	}
}

ViewObjectEnum ViewObject::GetType() const
{
	return this->_type;
}

void ViewObject::Show()
{
	this->_showFlag = true;
}

void ViewObject::Hide()
{
	this->_showFlag = false;
}

bool ViewObject::GetShow() const
{
	return this->_showFlag;
}



ViewObject::ViewObject(const ViewObjectEnum& type)
	:_type(type)
{
}
