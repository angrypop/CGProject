#pragma once
#include "ViewGroup.h"

// do shadow pass
class ShadowGroup : public ViewGroup, public std::enable_shared_from_this<ShadowGroup>
{
public:
	// prepare GL settings, must be called in Render()
	virtual void Prepare();
	// release GL settings, must be called in Render()
	virtual void Release();

	ShadowGroup();
	virtual void Init();


};

// do GBuffer rending pass
class GBufferGroup : public ViewGroup, public std::enable_shared_from_this<GBufferGroup>
{
public:
	// prepare GL settings, must be called in Render()
	virtual void Prepare();
	// release GL settings, must be called in Render()
	virtual void Release();

	GBufferGroup();
	virtual void Init();
protected:


};