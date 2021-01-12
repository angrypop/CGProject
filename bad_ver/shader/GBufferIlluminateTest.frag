#version 450 core

layout(location = 0) out vec4 fragColor;
in vec2 fragTextureCoord;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

float near = 0.1;
float far = 500.0f;

uniform mat4 uniP;
uniform mat4 uniV;

uniform sampler2D uniPositionSampler;
uniform sampler2D uniColorSampler;
uniform sampler2D uniDepthSampler;


void main() {
	// 从G缓冲中获取数据
	float specularStrength = 0.5;
	vec3 fragPos = texture(uniPositionSampler, fragTextureCoord).rgb;
	fragColor = texture(uniColorSampler, fragTextureCoord);
	//fragColor = vec4(reflectEffect(color.rgb, normal, fragTextureCoord, fragPos, texture(uniDepthSampler, fragTextureCoord).a), 1.0f);
	//fColor = vec4(attenuation * originalColor * (ambient + (diffuse + specular) * (1.0 - shadow)), 1.0f);
	//fColor = vec4(originalColor, 1.0);
	//fColor = vec4(texture(gDepthID, textureCoord).aaa, 1.0f);
	//fColor = vec4(normal, 1.0);
	//fColor = vec4(reflect(normalize(fragPos - viewPos), normal) * 0.5 + 0.5, 1.0f);
	//fColor = vec4(shadow * 1000);
	//fColor = vec4((P * V * vec4(fragPos, 1.0f)).zzzz/100);
	//vec4 temp = fragPosLightSpace / fragPosLightSpace.w * 0.5 + 0.5;
	//fColor = texture(shadowMap,temp.xy).rrrr;
}