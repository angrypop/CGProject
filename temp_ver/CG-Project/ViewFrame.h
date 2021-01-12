#pragma once
#include "ViewTexture.h"


class ViewFrame
{
public:
	GLuint GetFBO() const
	{
		return _FBO;
	}

	void AddColorAttachment(std::shared_ptr<ViewTexture> texturePtr, const GLuint& attachment, const GLint& level = 0)
	{
		::glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
		texturePtr->BindFrame(attachment, level);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->_colorAttachments.push_back(texturePtr);
	}

	void SetDepthAttachment(std::shared_ptr<ViewRenderBuffer> depthBuffer, const GLuint& attachment)
	{
		::glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
		depthBuffer->BindFrame(attachment);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->_depthBuffer = depthBuffer;
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
	std::shared_ptr<ViewRenderBuffer> _depthBuffer;
	std::vector<std::shared_ptr<ViewTexture>> _colorAttachments;
	GLuint _FBO;
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
	static GLuint AddFrame(const std::shared_ptr<ViewFrame> frame, const std::string& name = DefaultName.data());
protected:
	static std::map<std::string, GLuint> _nameFBOMap;
	static std::map<GLuint, std::shared_ptr<ViewFrame>> _FBOFrameMap;
	static constexpr std::string_view DefaultName = "___DEFAULT___";
};

class GBufferFrame final : public ViewFrame
{
public:
	GBufferFrame(const GLsizei& width, const GLsizei& height)
		:_width(width), _height(height)
	{
		this->Init();
	}
	void Init()
	{
		::glBindFramebuffer(GL_FRAMEBUFFER, this->GetFBO());

		std::shared_ptr<ViewTexture> ptrT;
		ptrT.reset(new ViewTexture(_width, _height, "uniPositionSampler"));
		this->AddColorAttachment(ptrT, GL_COLOR_ATTACHMENT0);
		ptrT.reset(new ViewTexture(_width, _height, "uniColorSampler"));
		this->AddColorAttachment(ptrT, GL_COLOR_ATTACHMENT1);
		std::shared_ptr<ViewRenderBuffer> ptrR;
		ptrR.reset(new ViewRenderBuffer(_width, _height, "uniDepthSampler"));
		this->SetDepthAttachment(ptrR, GL_DEPTH_ATTACHMENT);

		std::array<GLenum, 2> attachments { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, attachments.data());

		if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Bad Frame Buffer!" << std::endl;
		else
			std::cout << "GBuffer Config Completed" << std::endl;
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

protected:
	GLsizei _width, _height;


};