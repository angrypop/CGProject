#version 450

layout(location = 0) in vec3 position;

out vec3 textureCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
	gl_Position = P * V * vec4(position, 1.0f)*10000;
	textureCoord = position;
}