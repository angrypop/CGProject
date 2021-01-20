#version 450

layout(location = 0) in vec3 position;

out vec3 textureCoord;

uniform mat4 uniM;
uniform mat4 uniV;
uniform mat4 uniP;

void main() {
	gl_Position = uniP * uniV * vec4(position, 1.0f)*10000;
	textureCoord = position;
}