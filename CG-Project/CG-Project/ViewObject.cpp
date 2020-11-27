#include "ViewCppHeader.h"

void ViewObject::Render(const ViewModule& module)
{
	GLuint&& programHandle = module.GetProgramHandle();
	module.UseProgram();
	::glBindVertexArray(_VAO);
	this->UpdateData(programHandle);
	for (const auto& data : this->_uniformData)
	{
		data->Transfer(programHandle);
	}
	::glDrawArrays(module.GetDrawType(), 0, this->GetPointCount());
}

UniformDataBase* ViewObject::GetUniformData(std::string&& name) const
{
	for (const auto& data : this->_uniformData)
		if (data->GetName() == name)
			return data.get();
	return nullptr;
}


void ViewObject::AddUniformData(std::shared_ptr<UniformDataBase>&& uniformData)
{
	this->_uniformData.push_back(uniformData);
}

void ViewObject::AddBufferData(std::shared_ptr<BufferDataBase>&& bufferData)
{
	this->_bufferData.push_back(bufferData);
}

BufferDataBase* ViewObject::GetBufferData(const GLuint& position) const
{
	return this->_bufferData[position].get();
}

void ViewObject::Bind(const std::weak_ptr<ViewGroup>& groupPtr)
{
	this->_groupPtr = groupPtr;
}

const std::weak_ptr<ViewGroup>& ViewObject::Parent() const
{
	return this->_groupPtr;
}

void ViewObject::Hide()
{
	this->_showFlag = false;
}

void ViewObject::Show()
{
	this->_showFlag = true;
}

bool ViewObject::IsHide() const
{
	return !(this->_showFlag);
}

void ViewObject::SetType(const ViewObjectEnum& type)
{
	this->_type = type;
}

ViewObjectEnum ViewObject::GetType() const
{
	return this->_type;
}

GLsizei ViewObject::GetPointCount() const
{
	return this->_verNum;
}

bool ViewObject::operator==(const ViewObject& rhs)
{
	return this->_handle == rhs._handle;
}

bool ViewObject::operator<(const ViewObject& rhs)
{
	return this->_handle < rhs._handle;
}

bool ViewObject::operator>(const ViewObject& rhs)
{
	return this->_handle > rhs._handle;
}

ViewObject::~ViewObject()
{
	::glDeleteVertexArrays(1, &this->_VAO);
}


ViewTriangle::ViewTriangle(std::array<GLfloat, POINTSIZE * 3>&& vertices)
	: ViewObject(ViewObjectEnum::TriangleObject, 3)
{
	this->InitData(std::move(vertices));
}

void ViewTriangle::InitData(std::array<GLfloat, POINTSIZE * 3>&& vertices)
{
	// generate VAO
	::glGenVertexArrays(1, &_VAO);

	// add buffer data
	std::vector<GLsizei> bufferInfo = std::vector<GLsizei>{ COORDSIZE, COLORSIZE };
	this->AddBufferData(std::shared_ptr<BufferDataBase>(new BufferData<GLfloat, 3, POINTSIZE>(vertices, bufferInfo)));

	// add uniform data
	glm::mat4 trans = glm::mat4(1.0);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	this->AddUniformData(std::shared_ptr<UniformDataBase>(new UniformData<glm::mat4>(
		"uniRotate", trans)));

	// bind bufferdata
	for (const auto& data : _bufferData)
	{
		data->BindVAO(_VAO);
	}
	// no need to bind uniform data since we will do it later in UpdateData
}

void ViewTriangle::UpdateData(const GLuint& programHandle)
{
	// change and update uniform data
	_transAngle += 1.0f;
	glm::mat4 trans = glm::rotate(glm::mat4(1.0), glm::radians(_transAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	auto trans_ptr = dynamic_cast<UniformData<glm::mat4>*>(this->GetUniformData("uniRotate"));
	trans_ptr->SetData(std::move(trans));
}



