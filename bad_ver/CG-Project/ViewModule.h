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
	// use AddUniformName() to declare uniform variable names
	virtual void InitUniform() = 0;
// non-pure virtual function zone
public:
	//// must be called before ViewObject::Render()
	//virtual void Prepare();
	//// must be called after ViewObject::Render()
	//virtual void Release();

// other function zone
public:

	// add a shader to this program by filename and mode
	void AddShader(const std::string & filename, const GLenum & mode);
	// add a uniform variable name to this program
	void AddUniformName(const std::string& name);

	// check if this uniform variable is needed
	bool CheckUniformName(const std::string& name) const;
	// get number of all uniform name
	size_t GetUniformNameNumber() const;

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
	std::set<std::string> _uniforms;
	GLuint _programHandle = 0;
	GLuint _drawType;
};

class TestModule : public ViewModule
{
public:
	//virtual void Prepare();
	TestModule();
	virtual void InitShader();
	virtual void InitUniform();
};

class GBufferIlluminateModule : public ViewModule
{
public:
	//virtual void Prepare();
	GBufferIlluminateModule();
	virtual void InitShader();
};



//class GBufferIlluminateModule : public ViewModule
//{
//public:
//	//virtual void Prepare();
//	GBufferIlluminateModule();
//	virtual void InitShader();
//};
//
//class GBufferIlluminateModule : public ViewModule
//{
//public:
//	//virtual void Prepare();
//	GBufferIlluminateModule();
//	virtual void InitShader();
//};