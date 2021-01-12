#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordVert; 

out vec2 textureCoordFrag;

void main()
{
	textureCoordFrag = textureCoordVert;
	gl_Position = vec4(position, 1.0);
}