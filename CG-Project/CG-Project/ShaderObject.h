#pragma once
#include "ViewBase.h"

// shader class needs not to derive from ViewBase
// since the handle of shader is given by OpenGL instead of us
class ShaderObject
{
// other function zone
public:
	// create shader object by filename and mode, mode could be GL_VERTEX_SHADER etc.
	ShaderObject(const std::string& filename, const GLenum& mode);
	virtual ~ShaderObject() {}

	// load shader by filename and mode
	void Load(const std::string& filename, const GLenum& mode);
	// get shader mode
	const GLenum& GetMode() const;
	// get shader filename
	const std::string& GetFilename() const;
	// get shader handle given by OpenGL
	const GLuint& GetHandle() const;
	// compare with handle
	bool operator < (const ShaderObject& rhs) const;

// data zone
protected:
	GLenum _mode = 0;
	std::string _filename;
	GLuint _handle = 0;

};

class ShaderPool final
{
// other function zone
public:
	// check whether shader with filename has been loaded into pool
	// return 0 if nonexisted, return handle of shader is existed
	static GLuint CheckExist(const std::string& filename);

	// check whether shader with handle has been loaded into pool
	// return 0 if nonexisted, return handle of shader is existed
	static GLuint CheckExist(const GLuint& handle);

	// load shader with filename and shader mode, if existed will skip loading
	// return handle of shader
	static GLuint Load(const std::string& filename, const GLenum& mode);

// data zone
protected:
	static std::set<ShaderObject> _shaders;
};
