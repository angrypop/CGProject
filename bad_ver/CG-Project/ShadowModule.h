#pragma once
#include "ViewModule.h"


class ShadowGrassModule : public ViewModule
{
public:
	ShadowGrassModule();
	virtual void InitShader();
	virtual void InitUniform();
};


class ShaderWaterModule : public ViewModule
{
public:
	ShaderWaterModule();
	virtual void InitShader();
 };