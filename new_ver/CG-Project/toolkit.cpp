#include "toolkit.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

GLuint LoadShader(const ShaderInfo& shaderInfo)
{
	std::string filename = ShaderPath.data() + shaderInfo.filename;
	if (shaderInfo.mode == GL_NONE)
		return GL_NONE;
	return ShaderPool::Load(filename, shaderInfo.mode);
}

GLuint GenerateProgram(const std::vector<ShaderInfo>& shaderInfos)
{
	GLuint program = glCreateProgram();
	for (const auto& shaderInfo : shaderInfos)
	{
		GLuint shader = LoadShader(shaderInfo);
		glAttachShader(program, shader);
	}
	glLinkProgram(program);
	return program;
}

GLuint LoadTexture(const TextureInfo& info)
{
	return TexturePool::Load(info.fileName);
}

std::vector<std::string> faces
{
	"skybox/right.jpg",
	"skybox/left.jpg",
	"skybox/top.jpg",
	"skybox/bottom.jpg",
	"skybox/front.jpg",
	"skybox/back.jpg"
};

GLuint loadCubemap(const std::vector<std::string> &faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::string filename = std::string(TexturePath) + faces[i];
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << filename << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void ScreenShot(const int & width, const int & height, const std::string &filename)
{
	constexpr int components = 3;
	constexpr int BMP_Header_Length = 54;
	int lengthW = (width * components + 3) / 4 * 4;
	std::cout << "Screen Shotting..." << std::endl;

	std::string&& realFilename = (std::string(ScreenShotPath) + filename);
	std::cout << "\tScreen Shot Filename = " << realFilename << std::endl;
	char * imageBuf = (char*)malloc(lengthW * height * sizeof(char));
	if (!imageBuf)
		throw("ScreenShot:: Memory NOT ENOUGH");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels((GLint)0, (GLint)0, (GLint)width, (GLint)height,
		GL_RGB, GL_UNSIGNED_BYTE, imageBuf);
	
	// reverse the image

	for (int i = 0; i < height / 2; i++)
		for (int j = 0; j < lengthW; j++)
		{
			int pos = i * lengthW + j;
			int spos = (height - 1 - i) * lengthW + j;
			swap(imageBuf[pos], imageBuf[spos]);
		}

	// write the image into file
	stbi_write_jpg(realFilename.c_str(), width, height, 3, imageBuf, 0);
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
		std::cout << "\tFilename = " << filename << std::endl;
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

std::map<std::string, GLuint> TexturePool::_nameTBOMap;

GLuint TexturePool::Load(const std::string& filenameNoPath)
{
	std::string filename = std::string(TexturePath) + filenameNoPath;
	std::cout << "TexturePool Size = " << _nameTBOMap.size() << std::endl;
	if (_nameTBOMap.find(filename) != _nameTBOMap.end()) // found
		return _nameTBOMap.find(filename)->second;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		switch (nrChannels) {
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data); break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); break;
		default:
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("fail to load image");
	}

	_nameTBOMap[filename] = texture;
	return texture;
}
