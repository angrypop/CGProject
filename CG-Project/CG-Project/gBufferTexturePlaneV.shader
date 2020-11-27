#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 textureCoord;
out vec3 fragPos;
//out mat4 Model;

float getLinearDepthOfViewCoord(vec3 viewCoord) {
	vec4 p = vec4(viewCoord, 1.0);
	p = P * p;
	p /= p.w;
	return p.z;//linearizeDepth(p.z) / far;
}
out float depth;

void main() {
	gl_Position = P * V * M * position;
	textureCoord = vec2(texCoord.x, texCoord.y);

	fragPos = (M * position).xyz;
	vec4 viewPos = V * vec4(fragPos, 1.0f);
	depth = getLinearDepthOfViewCoord((viewPos / viewPos.w).xyz);
}