#include "ViewHeader.h"

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
	int index = 0;
	for (const auto& texture : this->_textures)
	{
		texture->Render(programHandle, index);
		index++;
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


void ViewObject::Rotate(const GLfloat& angle, const glm::vec3& axis)
{
	this->_M = glm::rotate(this->_M, glm::radians(angle), axis);
}

void ViewObject::Translate(const glm::vec3& displacement)
{
	this->_M = glm::translate(this->_M, displacement);
}

void ViewObject::Scale(const glm::vec3& scaler)
{
	this->_M = glm::scale(this->_M, scaler);
}

glm::mat4 ViewObject::GetM() const
{
	return this->_M;
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

void ViewObject::AddTexture(const std::string& name)
{
	std::shared_ptr<ViewTexture> texture = ViewTexturePool::Get(name);
	this->_textures.push_back(texture);
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
		"uniM", trans)));
	this->AddUniformData(std::shared_ptr<UniformDataBase>(new UniformData<glm::mat4>(
		"uniV", trans)));
	this->AddUniformData(std::shared_ptr<UniformDataBase>(new UniformData<glm::mat4>(
		"uniP", trans)));

	// add texture buffer, whose name is defined in GameWorld::InitTexture()
	this->AddTexture("ColorTexture");

	// bind bufferdata
	for (const auto& data : _bufferData)
	{
		data->BindVAO(_VAO);
	}
}

void ViewTriangle::UpdateData(const GLuint& programHandle)
{
	// change and update uniform data
	//_transAngle += 1.0f;
	//glm::mat4 trans = glm::rotate(glm::mat4(1.0), glm::radians(_transAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	dynamic_cast<UniformData<glm::mat4>*>(this->GetUniformData("uniM"))->SetData(std::move(this->_M));
	glm::mat4 V = UniformDataPool::GetData<glm::mat4>("V");
	dynamic_cast<UniformData<glm::mat4>*>(this->GetUniformData("uniV"))->SetData(std::move(V));
	glm::mat4 P = UniformDataPool::GetData<glm::mat4>("P");
	dynamic_cast<UniformData<glm::mat4>*>(this->GetUniformData("uniP"))->SetData(std::move(P));
}

ViewPolygon::ViewPolygon(const std::vector<GLfloat>& vertices)
	: ViewObject(ViewObjectEnum::PolygonObject, (GLsizei)vertices.size() / POINTSIZE)
{
	if (vertices.size() % (POINTSIZE * 3) != 0)
		throw(std::string("ViewPolygon:: vertices array size must be mutiple of (3 * POINTSIZE)"));
	this->InitData(vertices);
}

void ViewPolygon::UpdateData(const GLuint& programHandle)
{
	dynamic_cast<UniformData<glm::mat4>*>(this->GetUniformData("uniM"))->SetData(std::move(this->_M));
	glm::mat4 V = UniformDataPool::GetData<glm::mat4>("V");
	dynamic_cast<UniformData<glm::mat4>*>(this->GetUniformData("uniV"))->SetData(std::move(V));
	glm::mat4 P = UniformDataPool::GetData<glm::mat4>("P");
	dynamic_cast<UniformData<glm::mat4>*>(this->GetUniformData("uniP"))->SetData(std::move(P));
}

void ViewPolygon::InitData(const std::vector<GLfloat>& vertices)
{
	// generate VAO
	::glGenVertexArrays(1, &_VAO);

	// add buffer data
	std::vector<GLsizei> bufferInfo = std::vector<GLsizei>{ COORDSIZE, COLORSIZE };
	this->AddBufferData(std::shared_ptr<BufferDataBase>(new BufferDataVector<GLfloat>(vertices, POINTSIZE, bufferInfo)));
	
	// add uniform data
	glm::mat4 trans = glm::mat4(1.0);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	this->AddUniformData(std::shared_ptr<UniformDataBase>(new UniformData<glm::mat4>(
		"uniM", trans)));
	this->AddUniformData(std::shared_ptr<UniformDataBase>(new UniformData<glm::mat4>(
		"uniV", trans)));
	this->AddUniformData(std::shared_ptr<UniformDataBase>(new UniformData<glm::mat4>(
		"uniP", trans)));

	// add texture buffer, whose name is defined in GameWorld::InitTexture()
	this->AddTexture("ColorTexture");

	// bind bufferdata
	for (const auto& data : _bufferData)
	{
		data->BindVAO(_VAO);
	}
}
