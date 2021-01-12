#include "ViewFrame.h"

std::map<std::string, GLuint> ViewFramePool::_nameFBOMap;
std::map<GLuint, std::shared_ptr<ViewFrame>> ViewFramePool::_FBOFrameMap;

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

GLuint ViewFramePool::AddFrame(const std::shared_ptr<ViewFrame> frame, const std::string& name)
{
	_FBOFrameMap.insert(std::pair<GLuint, std::shared_ptr<ViewFrame>>(frame->GetFBO(), frame));
	if (name == DefaultName)
	{
		std::string&& nameDefault = name + std::to_string(_nameFBOMap.size()) + "___";
		_nameFBOMap.insert(std::pair<std::string, GLuint>(nameDefault, frame->GetFBO()));
	}
	else
		_nameFBOMap.insert(std::pair<std::string, GLuint>(name, frame->GetFBO()));
	return frame->GetFBO();
}

GBufferFrame::GBufferFrame(const GLsizei& width, const GLsizei& height)
	:_width(width), _height(height)
{
	this->Init();
}

void GBufferFrame::Init()
{
	//::glBindFramebuffer(GL_FRAMEBUFFER, this->GetFBO());

	//std::shared_ptr<ViewTexture> ptrT;
	//ptrT.reset(new ViewTexture(_width, _height, "uniPositionSampler"));
	//this->AddColorAttachment(ptrT, GL_COLOR_ATTACHMENT0);
	//ptrT.reset(new ViewTexture(_width, _height, "uniColorSampler"));
	//this->AddColorAttachment(ptrT, GL_COLOR_ATTACHMENT1);
	//std::shared_ptr<ViewRenderBuffer> ptrR;
	//ptrR.reset(new ViewRenderBuffer(_width, _height, "uniDepthSampler"));
	//this->SetDepthAttachment(ptrR, GL_DEPTH_ATTACHMENT);

	//::glBindFramebuffer(GL_FRAMEBUFFER, this->GetFBO());
	//std::array<GLenum, 2> attachments{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	//glDrawBuffers(2, attachments.data());

	//// check frame buffer
	//if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "Bad Frame Buffer!" << std::endl;
	//else
	//	std::cout << "GBuffer Config Completed" << std::endl;
	//::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, this->GetFBO());

	glGenTextures(NumGTexture, GTextures);
	// - 位置颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GTextures[GPositionTexture], 0);

	// - 法线颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GTextures[GNormalTexture], 0);

	// - 颜色 + 镜面颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture], 0);

	// - 深度缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, GTextures[GDepthIDTexture], 0);

	// - 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << " " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


GLuint ViewFrame::GetFBO() const
{
	return _FBO;
}

//void ViewFrame::AddColorAttachment(std::shared_ptr<ViewTexture> texturePtr, const GLuint& attachment, const GLint& level)
//{
//	::glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
//	texturePtr->BindFrame(attachment, level);
//	//::glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	this->_colorAttachments.push_back(texturePtr);
//}
//
//void ViewFrame::SetDepthAttachment(std::shared_ptr<ViewRenderBuffer> depthBuffer, const GLuint& attachment)
//{
//	::glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
//	depthBuffer->BindFrame(attachment);
//	::glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	this->_depthBuffer = depthBuffer;
//}

ViewFrame::ViewFrame()
{
	::glGenFramebuffers(1, &_FBO);
	::glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ViewFrame::~ViewFrame()
{
	::glDeleteFramebuffers(1, &_FBO);
}

ShadowFrame::ShadowFrame(const GLsizei& width, const GLsizei& height)
{
	this->Init();
}

void ShadowFrame::Init()
{
	glGenFramebuffers(NumShadowFrameBuffer, shadowFrameBuffers);

	glGenBuffers(NumShadowTextureBuffer, shadowBuffers);
	glBindTexture(GL_TEXTURE_2D, shadowBuffers[ShadowBuffer]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffers[ShadowFrameBuffer]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowBuffers[ShadowBuffer], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
