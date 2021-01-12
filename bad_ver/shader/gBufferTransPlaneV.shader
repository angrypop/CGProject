#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal_world;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec4 planeColor;

out vec3 normal;
out vec3 worldPos;
out vec4 color;
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

	normal = normal_world;

	worldPos = (M * position).xyz;
	vec4 viewPos = V * vec4(worldPos, 1.0f);
	depth = getLinearDepthOfViewCoord((viewPos / viewPos.w).xyz);

	color = planeColor;
}