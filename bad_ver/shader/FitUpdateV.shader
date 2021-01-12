#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 textureCoord;
out vec3 fragPos;

void main() {
	//gl_Position = vec4(0, 0, 0, 1);

	textureCoord = vec2(texCoord.x, texCoord.y);
	fragPos = (/*M **/ position).xyz;
	//gl_Position = P * V * M * position;
	gl_Position = vec4(2*(texCoord.x-0.5), 2*(texCoord.y-0.5), 0, 1);
}