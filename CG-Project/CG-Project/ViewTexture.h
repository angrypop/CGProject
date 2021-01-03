#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "ViewBase.h"
#include "ViewData.h"


class ViewTexture
{
public:
	GLuint GetTBO() const;

	ViewTexture(const GLsizei& width, const GLsizei& height, const std::string& samplerName,
		const GLint& internalformat = GL_RGB16F, const GLenum& format = GL_RGB);

	ViewTexture(const std::string& filename, const std::string& samplerName,
		const GLint& internalformat = GL_RGB16F, const GLenum& format = GL_RGB);

	// bind this texture to given attachment and level in CURRENT Frame Buffer
	// e.g. BindFBO(GL_DEPTH_ATTACHMENT, 0)
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
	std::shared_ptr<UniformData<GLint>> _sampler;
private:
	GLuint _TBO;
	//static GLuint _TextureCnt;
};




class ViewFrame
{
public:
	GLuint GetFBO() const
	{
		return _FBO;
	}

	GLuint AddTexture(std::shared_ptr<ViewTexture> texturePtr, const GLuint& attachment, const GLint& level = 0)
	{
		::glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
		texturePtr->BindFrame(attachment, level);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ViewFrame()
	{
		::glGenFramebuffers(1, &_FBO);
		::glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual ~ViewFrame()
	{
		::glDeleteFramebuffers(1, &_FBO);
	}

protected:
	GLuint _FBO;
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

class ViewFramePool
{
public:
	// get FBO of a frame by its name
	static GLuint GetFrameFBO(const std::string& name);
	// get frame pointer by its FBO
	static std::shared_ptr<ViewFrame> GetFrame(const GLuint& FBO);
	// get frame pointer by its name
	static std::shared_ptr<ViewFrame> GetFrame(const std::string& name);
	// add a frame with name(alternative), return its FBO
	static GLuint AddFrame(const std::string& name = DefaultName.data());
protected:
	static std::map<std::string, GLuint> _nameFBOMap;
	static std::map<GLuint, std::shared_ptr<ViewFrame>> _FBOFrameMap;
	static constexpr std::string_view DefaultName = "___DEFAULT___";
}; 
