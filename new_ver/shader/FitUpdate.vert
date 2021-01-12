#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 uniM;
uniform mat4 uniV;
uniform mat4 uniP;

out vec2 textureCoord;
out vec3 fragPos;

void main() {
	//gl_Position = vec4(0, 0, 0, 1);

	textureCoord = vec2(texCoord.x, texCoord.y);
	fragPos = (/*uniM **/ position).xyz;
	//gl_Position = uniP * uniV * uniM * position;
	gl_Position = vec4(2*(texCoord.x-0.5), 2*(texCoord.y-0.5), 0, 1);
}