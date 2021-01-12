#include "ViewHeader.h"



void ViewModule::AddShader(const std::string& filename, const GLenum& mode)
{
	std::string realFilename = std::string("..\\shader\\") + filename;
	GLuint shaderHandle = ShaderPool::Load(realFilename, mode);
	this->_shaders.push_back(shaderHandle);
	::glAttachShader(this->_programHandle, shaderHandle);
}

void ViewModule::AddUniformName(const std::string& name)
{
	this->_uniforms.insert(name);
}

bool ViewModule::CheckUniformName(const std::string& name) const
{
	if (this->_uniforms.find(name) != _uniforms.end())
		return true;
	return false;
}

size_t ViewModule::GetUniformNameNumber() const
{
	return this->_uniforms.size();
}

GLuint ViewModule::GetDrawType() const
{
	return this->_drawType;
}

GLuint ViewModule::GetProgramHandle() const
{
	return this->_programHandle;
}

void ViewModule::UseProgram() const
{
	if (this->_programHandle == 0)
		throw std::exception("This Program has not been initialized");
	::glUseProgram(this->_programHandle);
}


TestModule::TestModule()
	: ViewModule(GL_TRIANGLES)
{
	this->_programHandle = ::glCreateProgram();
	this->InitShader();
	::glLinkProgram(this->_programHandle);
	this->InitUniform();
}

void TestModule::InitShader()
{
	// add shader by filename and mode
	this->AddShader("Triangle.vert", GL_VERTEX_SHADER);
	this->AddShader("Triangle.frag", GL_FRAGMENT_SHADER);
}

void TestModule::InitUniform()
{
	// add uniform variable name
	this->AddUniformName("uniM");
	this->AddUniformName("uniV");
	this->AddUniformName("uniP");
}


//void GBufferIlluminateModule::Prepare()
//{
//	this->ViewModule::Prepare();
//	ViewFramePool::GetFrame("GBuffer")->Prepare(this->GetProgramHandle());
//}

GBufferIlluminateModule::GBufferIlluminateModule()
	: ViewModule(GL_TRIANGLE_STRIP)
{
	this->_programHandle = ::glCreateProgram();
	this->InitShader();
	::glLinkProgram(this->_programHandle);
	//this->InitUniform();
}

void GBufferIlluminateModule::InitShader()
{
	this->AddShader("GBufferIlluminate.vert", GL_VERTEX_SHADER);
	this->AddShader("GBufferIlluminate.frag", GL_FRAGMENT_SHADER);
}


