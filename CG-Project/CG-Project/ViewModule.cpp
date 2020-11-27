#include "ViewCppHeader.h"



void ViewModule::AddShader(std::string filename, GLenum mode)
{
	GLuint shaderHandle = ShaderPool::Load(filename, mode);
	this->_shaders.push_back(shaderHandle);
	::glAttachShader(this->_programHandle, shaderHandle);
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
}

void TestModule::InitShader()
{
	// add shader by filename and mode
	this->AddShader("Triangle.vert", GL_VERTEX_SHADER);
	this->AddShader("Triangle.frag", GL_FRAGMENT_SHADER);
}
