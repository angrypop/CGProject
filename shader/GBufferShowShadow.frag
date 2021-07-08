#version 450 core

layout(location = 0) out vec4 fColor;
in vec2 textureCoord;

uniform float uniNear;
uniform float uniFar;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * uniNear * uniFar) / (uniFar + uniNear - z * (uniFar - uniNear));
}

uniform sampler2D shadowMap;

void main() {
	//fColor = vec4(vec3(texture(shadowMap, textureCoord).r / uniFar), 1.0);
	fColor = vec4(vec3(texture(shadowMap, textureCoord).r), 1.0);
}