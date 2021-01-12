#pragma once
#include "ViewTexture.h"


class ViewFrame
{
public:
	// send uniform data to GPU
	//virtual void Prepare(const GLuint & programHandle);

public:
	GLuint GetFBO() const;

	//void AddColorAttachment(std::shared_ptr<ViewTexture> texturePtr, const GLuint& attachment, const GLint& level = 0);

	//void SetDepthAttachment(std::shared_ptr<ViewRenderBuffer> depthBuffer, const GLuint& attachment);

	ViewFrame();

	virtual ~ViewFrame();



protected:
	//std::shared_ptr<ViewRenderBuffer> _depthBuffer;
	//std::vector<std::shared_ptr<ViewTexture>> _colorAttachments;
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
	GBufferFrame(const GLsizei& width, const GLsizei& height);
	void Init();
	//virtual void Prepare();

protected:
	GLsizei _width, _height;
	enum { GBuffer, NumBuffer };
	enum { GPositionTexture, GNormalTexture, GAlbedoSpecTexture, GDepthIDTexture, NumGTexture };
	GLuint GTextures[NumGTexture];

	//const int StroageSize = 2048 * 2048;
	//enum { HeadPointerInit, Storage, StorageWorldPos, StorageNormal, AtomicCounter, NumOITBuffer };
	//enum { HeadPointerTexture, StorageTexture, StorageWorldPosTexture, StorageNormalTexture, NumOITTexture };
	//enum { WaterOIT, RankOIT, DisplayOIT, PlaneOIT, NumOITProgram };
	//GLuint OITBuffers[NumOITBuffer];
	//GLuint OITTextures[NumOITTexture];
	//GLuint OITPrograms[NumOITProgram];

	//GLuint cubemapTexture;

};

class ShadowFrame final : public ViewFrame
{
public:
	ShadowFrame(const GLsizei& width, const GLsizei& height);
	void Init();
protected:
	enum { ShadowBuffer, NumShadowTextureBuffer };
	enum { ShadowFrameBuffer, NumShadowFrameBuffer };
	enum { PlaneProgram, WaterProgram, GrassProgram, DisplayShadowProgram, FitPlaneShadowProgram, NumShadowProgram };
	GLuint shadowBuffers[NumShadowTextureBuffer];
	GLuint shadowFrameBuffers[NumShadowFrameBuffer];
	GLuint shadowPrograms[NumShadowProgram];
	const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
};