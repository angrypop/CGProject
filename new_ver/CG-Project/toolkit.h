#pragma once

#include "ViewHeader.h"
//---------------------------------------------------------------------------------SHADER

constexpr std::string_view ShaderPath = "..\\shader\\";

struct ShaderInfo {
	int mode;
	std::string filename;
};

GLuint LoadShader(const ShaderInfo &shaderInfo);

GLuint GenerateProgram(const std::vector<ShaderInfo>& shaderInfos);
//---------------------------------------------------------------------------------TEXTURE
enum { AmbientTexture, NormalTexture, RoughnessTexture , Texture_NONE};
struct TextureInfo {
	int mode;
	std::string fileName;
};

GLuint LoadTexture(const TextureInfo& info);
//---------------------------------------------------------------------------------CUBE BOX
extern std::vector<std::string> faces;
GLuint loadCubemap(const std::vector<std::string>& faces);