#pragma once

#include "ViewBase.h"
#include "ViewData.h"

class ViewBufferObject
{
public:
	GLuint GetBO() const;
	ViewBufferObject(const std::string& samplerName)
	{
		_sampler.reset(new UniformData<GLint>(samplerName, 0));
	}

protected:
	std::shared_ptr<UniformData<GLint>> _sampler;
	GLuint _BO = 0;
};

class ViewTexture : public ViewBufferObject
{
public:


	ViewTexture(const GLsizei& width, const GLsizei& height, const std::string& samplerName,
		const GLint& internalformat = GL_RGB16F, const GLenum& format = GL_RGB);

	ViewTexture(const std::string& filename, const std::string& samplerName,
		const GLint& internalformat = GL_RGB16F, const GLenum& format = GL_RGB);

	// bind this texture to given attachment and level in CURRENT Frame Buffer
	// e.g. BindFBO(GL_COLOR_ATTACHMENT0, 0)
	void BindFrame(const GLuint& attachment, const GLint& level = 0);
	
	// send sampler to GPU
	void Render(const GLuint& programHandle, const GLint& index);

	// read a picture as 2D texture
	void ReadPicture(const std::string& filename);

	// initialize texture settings
	void Init(const std::string& samplerName, const GLint& internalformat = GL_RGB16F, const GLenum& format = GL_RGB);
	
	virtual ~ViewTexture();
protected:
	GLsizei _width, _height;


	//static GLuint _TextureCnt;
};

class ViewRenderBuffer : public ViewBufferObject
{
public:
	ViewRenderBuffer (const GLsizei& width, const GLsizei& height, const std::string& samplerName);
	~ViewRenderBuffer();
	// bind this texture to given attachment and level in CURRENT Frame Buffer
	// e.g. BindFBO(GL_DEPTH_ATTACHMENT)
	void BindFrame(const GLuint& attachment);
protected:
	GLsizei _width, _height;

};


class ViewTexturePool
{
public:
	// get TBO of a frame by its name
	static GLuint GetBO(const std::string& name);
	// get texture pointer by its TBO
	static std::shared_ptr<ViewTexture> Get(const GLuint& TBO);
	// get texture pointer by its name
	static std::shared_ptr<ViewTexture> Get(const std::string& name);
	// add a texture with name(alternative), return its TBO
	static GLuint Add(const std::shared_ptr<ViewTexture> & texture, const std::string& name = DefaultName.data());
	// delete a texture by its FBO
	static void Delete(const GLuint& TBO);
protected:
	static std::map<std::string, GLuint> _nameBOMap;
	static std::map<GLuint, std::shared_ptr<ViewTexture>> _BOPointerMap;
	static constexpr std::string_view DefaultName = "___DEFAULT___";
};

