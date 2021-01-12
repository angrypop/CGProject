#version 450 core

layout(location = 0) in vec4 position;

uniform mat4 uniM;
uniform mat4 uniV;
uniform mat4 uniP;

void main() {
	gl_Position = uniP * uniV * uniM * position;
}