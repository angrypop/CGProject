#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal_world;

uniform mat4 uniM;
uniform mat4 uniV;
uniform mat4 uniP;
uniform vec4 planeColor;

out vec3 normal;
out vec3 worldPos;
out vec4 color;
//out mat4 Model;

float getLinearDepthOfViewCoord(vec3 viewCoord) {
	vec4 uniP = vec4(viewCoord, 1.0);
	uniP = uniP * uniP;
	uniP /= uniP.w;
	return uniP.z;//linearizeDepth(uniP.z) / far;
}
out float depth;

void main() {
	gl_Position = uniP * uniV * uniM * position;

	normal = normal_world;

	worldPos = (uniM * position).xyz;
	vec4 viewPos = uniV * vec4(worldPos, 1.0f);
	depth = getLinearDepthOfViewCoord((viewPos / viewPos.w).xyz);

	color = planeColor;
}