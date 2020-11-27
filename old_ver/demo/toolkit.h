#pragma once
//---------------------------------------------------------------------------------SHADER
struct ShaderInfo {
	int mode;
	std::string fileName;
};
GLuint LoadShader(ShaderInfo shaders) {
	GLuint shader = glCreateShader(shaders.mode);

	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shaderFile.open(shaders.fileName);

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
	glShaderSource(shader, 1, &shaderSource, nullptr);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << shaders.fileName + "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}
//---------------------------------------------------------------------------------TEXTURE
enum { AmbientTexture, NormalTexture, RoughnessTexture , Texture_NONE};
struct TextureInfo {
	int mode;
	std::string fileName;
};