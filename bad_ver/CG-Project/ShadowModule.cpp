#include "ShadowModule.h"

ShadowGrassModule::ShadowGrassModule()
	: ViewModule(GL_TRIANGLE_STRIP)
{
	this->_programHandle = ::glCreateProgram();
	this->InitShader();
	::glLinkProgram(this->_programHandle);
	this->InitUniform();
}

void ShadowGrassModule::InitShader()
{
	this->AddShader("GBufferTexturePlane.vert", GL_VERTEX_SHADER);
	this->AddShader("Control.tesc", GL_TESS_CONTROL_SHADER);
	this->AddShader("Evaluation.tese", GL_TESS_EVALUATION_SHADER);
	this->AddShader("Normal.geom", GL_GEOMETRY_SHADER);
	this->AddShader("ShadowG.frag", GL_FRAGMENT_SHADER);
}

void ShadowGrassModule::InitUniform()
{
	this->AddUniformName("uniM");
	this->AddUniformName("uniV");
	this->AddUniformName("uniP");
	this->AddUniformName("uniTime");
	this->AddUniformName("uniObjPos");
}


ShaderWaterModule::ShaderWaterModule()
	: ViewModule(GL_TRIANGLE_STRIP)
{
	this->_programHandle = ::glCreateProgram();
	this->InitShader();
	::glLinkProgram(this->_programHandle);
}

void ShaderWaterModule::InitShader()
{
	this->AddShader("gBufferWaterV.shader", GL_VERTEX_SHADER);
	this->AddShader("shadowF.shader", GL_FRAGMENT_SHADER);
}
