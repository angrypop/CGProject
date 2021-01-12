#version 450 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec4 gDepthID;

in vec3 textureCoord;
uniform samplerCube skybox;

float near = 0.1;
float far = 500.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 hsv2rgb(vec3 c) {
	const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 rgb2hsv(vec3 c) {
	const vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + 0.001)), d / (q.x + 0.001), q.x);
}

// hue: 色调; saturation: 饱和度; value: 明度
vec3 AdjustColor(vec3 rgbColor, float hue, float saturation, float value)
{
	vec3 hsvColor = rgb2hsv(rgbColor);
	return hsv2rgb(hsvColor + vec3(hue, saturation, value));
}

void main() {
	// 存储第一个G缓冲纹理中的片段位置向量
	gPosition = textureCoord*10000;
	// 同样存储对每个逐片段法线到G缓冲中
	gNormal = vec3(0, 0, 0);
	// 和漫反射对每个逐片段颜色
	//gAlbedoSpec.rgb = texture(skybox, textureCoord).rgb;
	gAlbedoSpec.rgb = AdjustColor(texture(skybox, textureCoord).rgb, 0.0, 0.1, 0.2);
	// 存储镜面强度到gAlbedoSpec的alpha分量
	gAlbedoSpec.a = 0;

	gDepthID.rgb = vec3(10000);
	gDepthID.a = 2;
}