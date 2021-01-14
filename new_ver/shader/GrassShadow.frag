#version 450 core

uniform float uniNear;
uniform float uniFar;

//layout(location = 0) out vec4 fColor;
in float depth_GS_FS;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * uniNear * uniFar) / (uniFar + uniNear - z * (uniFar - uniNear));
}

void main() {
	//gl_FragDepth = LinearizeDepth(gl_FragCoord.z) / uniFar;
	//gl_FragDepth = LinearizeDepth(depth_GS_FS) / 200.f;
	//fColor = vec4(LinearizeDepth(gl_FragCoord.z) / uniFar * 500);
	//fColor = vec4(gl_FragCoord.z);
}