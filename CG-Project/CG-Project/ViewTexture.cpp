#include "ViewTexture.h"

std::map<std::string, GLuint> ViewTexturePool::_nameBOMap;
std::map<GLuint, std::shared_ptr<ViewTexture>> ViewTexturePool::_BOPointerMap;

std::map<std::string, GLuint> ViewFramePool::_nameFBOMap;
std::map<GLuint, std::shared_ptr<ViewFrame>> ViewFramePool::_FBOFrameMap;

GLuint ViewTexturePool::GetBO(const std::string& name)
{
	auto target = _nameBOMap.find(name);
	if (target != _nameBOMap.end())
		return target->second;
	else
		throw(std::string("No Texture Name = ") + name);
}

std::shared_ptr<ViewTexture> ViewTexturePool::Get(const GLuint& TBO)
{
	auto target = _BOPointerMap.find(TBO);
	if (target != _BOPointerMap.end())
		return target->second;
	else
		throw(std::string("No Texture TBO = ") + std::to_string(TBO));
}

std::shared_ptr<ViewTexture> ViewTexturePool::Get(const std::string& name)
{
	return Get(GetBO(name));
}

GLuint ViewTexturePool::Add(const std::shared_ptr<ViewTexture>& texture, const std::string& name)
{
	std::shared_ptr<ViewTexture> ptr = texture;
	_BOPointerMap.insert(std::pair<GLuint, std::shared_ptr<ViewTexture>>(ptr->GetTBO(), ptr));
	if (name == DefaultName)
	{
		std::string&& nameDefault = name + std::to_string(_nameBOMap.size()) + "___";
		_nameBOMap.insert(std::pair<std::string, GLuint>(nameDefault, ptr->GetTBO()));
	}
	else
		_nameBOMap.insert(std::pair<std::string, GLuint>(name, ptr->GetTBO()));
	return ptr->GetTBO();
}

void ViewTexturePool::Delete(const GLuint& TBO)
{
	auto target = _BOPointerMap.find(TBO);
	if (target != _BOPointerMap.end())
		_BOPointerMap.erase(target);
	else
		throw(std::string("No Texture TBO = ") + std::to_string(TBO));
}


GLuint ViewFramePool::GetFrameFBO(const std::string& name)
{
	auto target = _nameFBOMap.find(name);
	if (target != _nameFBOMap.end())
		return target->second;
	else
		throw(std::string("No Frame Name = ") + name);
}

std::shared_ptr<ViewFrame> ViewFramePool::GetFrame(const GLuint& FBO)
{
	auto target = _FBOFrameMap.find(FBO);
	if (target != _FBOFrameMap.end())
		return target->second;
	else
		throw(std::string("No Frame FBO = ") + std::to_string(FBO));
}

std::shared_ptr<ViewFrame> ViewFramePool::GetFrame(const std::string& name)
{
	return GetFrame(GetFrameFBO(name));
}

GLuint ViewFramePool::AddFrame(const std::string& name)
{
	std::shared_ptr<ViewFrame> ptr(new ViewFrame());
	_FBOFrameMap.insert(std::pair<GLuint, std::shared_ptr<ViewFrame>>(ptr->GetFBO(), ptr));
	if (name == DefaultName)
	{
		std::string&& nameDefault = name + std::to_string(_nameFBOMap.size()) + "___";
		_nameFBOMap.insert(std::pair<std::string, GLuint>(nameDefault, ptr->GetFBO()));
	} else
		_nameFBOMap.insert(std::pair<std::string, GLuint>(name, ptr->GetFBO()));
	return ptr->GetFBO();
}

GLuint ViewTexture::GetTBO() const
{
	return this->_TBO;
}

ViewTexture::ViewTexture(const GLsizei& width, const GLsizei& height, const std::string& samplerName, const GLint& internalformat, const GLenum& format)
{
	this->Init(samplerName, internalformat, format);
	this->_width = width;
	this->_height = height;
	::glBindTexture(GL_TEXTURE_2D, this->_TBO);
	::glTexImage2D(GL_TEXTURE_2D, 0, internalformat, _width, _height, 0, format, GL_FLOAT, NULL);
	::glActiveTexture(GL_TEXTURE0);
	//glBufferData(GL_PIXEL_UNPACK_BUFFER, _width * _height * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	//GLuint *data = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	//memset(data, 0x00, width * height * sizeof(GLuint));
	//glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	::glBindTexture(GL_TEXTURE_2D, 0);
}

ViewTexture::ViewTexture(const std::string& filename, const std::string& samplerName, const GLint& internalformat, const GLenum& format)
{
	this->Init(samplerName, internalformat, format);

	this->ReadPicture(filename);
}

void ViewTexture::BindFrame(const GLuint& attachment, const GLint& level)
{
	::glBindTexture(GL_TEXTURE_2D, this->_TBO);

	::glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _TBO, level);

	::glBindTexture(GL_TEXTURE_2D, 0);
}

void ViewTexture::Render(const GLuint& programHandle, const GLint& index)
{
	::glActiveTexture(GL_TEXTURE0 + index);
	::glBindTexture(GL_TEXTURE_2D, this->_TBO);
	// set texture index to sampler
	_sampler->SetData((GLuint&&)index);
	_sampler->Transfer(programHandle);
}

void ViewTexture::ReadPicture(const std::string& filename)
{
	::glBindTexture(GL_TEXTURE_2D, this->_TBO);

	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nrChannels;
	unsigned char* image = ::stbi_load(filename.c_str(), &_width, &_height, &nrChannels, 0);
	if (image) // read successful
	{
		switch (nrChannels)
		{
		case 1:
			::glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, _width, _height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
			break;
		case 3:
			::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			break;
		case 4:
			::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			break;
		default:
			break;
		}
		::glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		throw(std::string("fail to load image"));
	}
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ViewTexture::Init(const std::string& samplerName, const GLint& internalformat, const GLenum& format)
{

	::glGenTextures(1, &_TBO);
	::glBindTexture(GL_TEXTURE_2D, _TBO);
	//::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	_sampler.reset(new UniformData<GLint>(samplerName, 0));
	//_sampler.reset(new UniformData<GLint>(samplerName, _TBO));
	::glBindTexture(GL_TEXTURE_2D, 0);
}

ViewTexture::~ViewTexture()
{
	::glDeleteTextures(1, &_TBO);
}
