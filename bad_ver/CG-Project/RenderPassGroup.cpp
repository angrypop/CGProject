#include "RenderPassGroup.h"

void GBufferGroup::Prepare()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ViewFramePool::GetFrameFBO("GBuffer"));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
}

void GBufferGroup::Release()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBufferGroup::GBufferGroup()
{
	this->Init();
}

void GBufferGroup::Init()
{
	// initialize objects

	// initialize modules
	auto testModule = std::shared_ptr<TestModule>(new TestModule());
	//testModule->SetFBO(ViewFramePool::GetFrameFBO("GBuffer"), ViewFramePool::GetFrameFBO("GBuffer"));
	this->AddModule(testModule, "test");
}
