#pragma once

#include "ViewHeader.h"
//---------------------------------------------------------------------------------SHADER

constexpr std::string_view ShaderPath = "..\\shader\\";

struct ShaderInfo {
	int mode;
	std::string filename;
};

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

GLuint LoadShader(const ShaderInfo &shaderInfo);

GLuint GenerateProgram(const std::vector<ShaderInfo>& shaderInfos);
//---------------------------------------------------------------------------------TEXTURE
enum { AmbientTexture, NormalTexture, RoughnessTexture , Texture_NONE};
struct TextureInfo {
	int mode;
	std::string fileName;
};

// pool of 2D-texture readed from disk
class TexturePool final
{
public:
	static GLuint Load(const std::string& filename);
protected:
	static std::map<std::string, GLuint> _nameTBOMap;
};

GLuint LoadTexture(const TextureInfo& info);
//---------------------------------------------------------------------------------CUBE BOX
extern std::vector<std::string> faces;
GLuint loadCubemap(const std::vector<std::string>& faces);
//---------------------------------------------------------------------------------SCREEN SHOT

//screen shot in given width, height and filename
void ScreenShot(const int& width, const int& height, const std::string& filename);