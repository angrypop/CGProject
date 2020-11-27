#include "ViewCppHeader.h"

std::set<ShaderObject> ShaderPool::_shaders;

ShaderObject::ShaderObject(const std::string& filename, const GLenum& mode)
{
	this->Load(filename, mode);
}

void ShaderObject::Load(const std::string& filename, const GLenum& mode)
{
	_filename = filename;
	_mode = mode;
	_handle = ::glCreateShader(mode);

	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shaderFile.open(filename);

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	int success;
	char infoLog[512];

	const char* shaderSource = shaderCode.c_str();
	//std::cout << "Shader " << filename << " content: " << shaderSource << std::endl;
	::glShaderSource(_handle, 1, &shaderSource, nullptr);

	::glCompileShader(_handle);

	::glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);
	if (!success) {
		::glGetShaderInfoLog(_handle, 512, nullptr, infoLog);
		std::cout << /*shaders.fileName +*/ "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

const GLenum& ShaderObject::GetMode() const
{
	return this->_mode;
}

const std::string& ShaderObject::GetFilename() const
{
	return this->_filename;
}

const GLuint& ShaderObject::GetHandle() const
{
	return this->_handle;
}

bool ShaderObject::operator<(const ShaderObject& rhs) const
{
	return this->_filename < rhs._filename;
}

GLuint ShaderPool::CheckExist(const std::string& filename)
{
	for (const auto& o : _shaders)
		if (o.GetFilename() == filename)
			return o.GetHandle();
	return 0;
}

GLuint ShaderPool::CheckExist(const GLuint& handle)
{
	for (const auto& o : _shaders)
		if (o.GetHandle() == handle)
			return handle;
	return 0;
}

GLuint ShaderPool::Load(const std::string& filename, const GLenum& mode)
{
	GLuint handle;
	if ((handle = CheckExist(filename)) != 0)
		return handle;
	ShaderObject temp(filename, mode);
	_shaders.insert(temp);
	return temp.GetHandle();
}
