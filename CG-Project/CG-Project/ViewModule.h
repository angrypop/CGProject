#pragma once
#include "ViewBase.h"

// shader class needs not to derive from ViewBase
// since the handle of shader is given by OpenGL instead of us
class ViewModule
{
// pure virtual function zone
public:
	// use AddShader() to initialize shaders orderly
	virtual void InitShader() = 0;

// non-pure virtual function zone
public:


// other function zone
public:
	// add a shader to this program by filename and mode
	void AddShader(std::string filename, GLenum mode);
	// get draw type in glDrawArrays()
	GLuint GetDrawType() const;
	// return the program handle of this module
	GLuint GetProgramHandle() const;
	// use this program in OpenGL
	void UseProgram() const;


	ViewModule(const int& drawType)
		: _drawType(drawType)
	{}
	virtual ~ViewModule() {}

// data zone
protected:
	std::vector<GLuint> _shaders;
	GLuint _programHandle = 0;
	GLuint _drawType;
};

class TestModule : public ViewModule
{
public:
	TestModule();
	virtual void InitShader();
};