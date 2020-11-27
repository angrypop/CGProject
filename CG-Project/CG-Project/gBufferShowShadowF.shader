#version 450 core

layout(location = 0) out vec4 fColor;
in vec2 textureCoord;

float near = 0.1;
float far = 1000.0;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));
}

uniform sampler2D shadowMap;

void main() {
	//fColor = vec4(vec3(texture(shadowMap, textureCoord).r / far), 1.0);
	fColor = vec4(vec3(texture(shadowMap, textureCoord).r), 1.0);
}