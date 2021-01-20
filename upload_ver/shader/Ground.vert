#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 uniM;
uniform mat4 uniV;
uniform mat4 uniP;

out vec2 textureCoord;
out vec3 fragPos;

void main() {
	gl_Position = uniP * uniV * uniM * position;
	textureCoord = vec2(texCoord.x,texCoord.y);

	fragPos = (uniM * position).xyz;
}