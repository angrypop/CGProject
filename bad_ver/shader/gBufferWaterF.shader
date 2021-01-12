#version 450 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec4 gDepthID;

in vec3 worldPos;
in vec3 normal;
in float depth;

float near = 0.1;
float far = 500.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));
}
void main() {
	// 存储第一个G缓冲纹理中的片段位置向量
	gPosition = worldPos;
	// 同样存储对每个逐片段法线到G缓冲中
	gNormal = normal;
	// 和漫反射对每个逐片段颜色
	gAlbedoSpec.rgb = vec3(135 / 255.9, 206 / 255.9, 250 / 255.9);
	// 存储镜面强度到gAlbedoSpec的alpha分量
	gAlbedoSpec.a = 0;

	gDepthID.rgb = vec3(LinearizeDepth(gl_FragCoord.z) / far);
	//gDepthID.rgb = vec3(gl_FragCoord.z);
	//gDepthID.rgb = vec3(getLinearDepthOfViewCoord(worldPos));
	//gDepthID.rgb = vec3(LinearizeDepth(depth) / far);
	gDepthID.a = 1;
}