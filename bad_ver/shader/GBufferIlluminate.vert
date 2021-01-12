#version 450 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertTextureCoord; 

out vec2 fragTextureCoord;

void main()
{
	fragTextureCoord = vertTextureCoord;
	gl_Position = vec4(vertPosition, 1.0);
}