#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 textureCoord;
out vec3 fragPos;

void main() {
	gl_Position = P * V * M * position;
	textureCoord = vec2(texCoord.x,texCoord.y);

	fragPos = (M * position).xyz;
}