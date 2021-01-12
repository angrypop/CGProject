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

uniform Light light;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix;
uniform mat4 uniP;
uniform mat4 uniV;

uniform sampler2D uniPositionSampler;
uniform sampler2D uniNormalSampler;
uniform sampler2D uniAlbedoSpecSampler;
uniform sampler2D uniDepthSampler;
uniform sampler2D uniShadowSampler;

float ShadowCalculation(float bias, vec4 fragPosLightSpace){
	// 执行透视除法
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// 变换到[0,1]的范围
	projCoords = projCoords * 0.5 + 0.5;
	// 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
	float closestDepth = texture(uniShadowSampler, projCoords.xy).r;
	// 取得当前片段在光源视角下的深度
	float currentDepth = projCoords.z;
	// 检查当前片段是否在阴影中
	float shadow = currentDepth - closestDepth > bias ? 1.0 : 0.0;
	fragColor = vec4(vec3(currentDepth - closestDepth), 1.0f);
	return shadow;
}
float linearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}
float getLinearDepthOfViewCoord(vec3 viewCoord) {
	vec4 p = vec4(viewCoord, 1.0);
	p = uniP * p;
	p /= p.w;
	return linearizeDepth(p.z) / far;
}
vec2 getScreenCoordByViewCoord(vec3 viewCoord) {
	vec4 p = vec4(viewCoord, 1.0);
	p = uniP * p;
	p /= p.w;
	if (p.z < -1 || p.z>1) return vec2(-1.0);
	p = p * 0.5 + 0.5;
	return p.st;
}
vec3 waterRayTarcing(vec3 startPoint, vec3 direction, vec3 color, float jitter, float schlick) {

	const float stepBase = 1;
	vec3 testPoint = startPoint;
	vec3 lastPoint = testPoint;
	direction *= stepBase;
	bool hit = false;
	vec4 hitColor = vec4(0.0);

	for (int i = 0; i < 160; i++) {
		testPoint += direction;
		vec2 uv = getScreenCoordByViewCoord((uniV * vec4(testPoint, 1.0f)).rgb);
		if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
			hit = false;
			break;
		}
		float sampleDepth = texture(uniDepthSampler, uv).r;
		float testDepth = getLinearDepthOfViewCoord((uniV * vec4(testPoint, 1.0f)).rgb);
		if (sampleDepth > 0.1 && sampleDepth < testDepth && testDepth - sampleDepth < (1.0 / 1024.0) * (1.0 + testDepth * 200 + float(i))) {
			vec3 finalPoint = lastPoint;
			float _sign = 1.0;
			vec3 tempDirection = testPoint - lastPoint;
			uv = getScreenCoordByViewCoord((uniV * vec4(finalPoint, 1.0f)).rgb);
			hitColor = vec4(texture(uniAlbedoSpecSampler, uv).rgb, 1.0);
			hitColor.a = clamp(1.0 - pow(distance(uv, vec2(0.5)) * 2.0, 2.0), 0.0, 1.0);
		}
		lastPoint = testPoint;
	}
	if (!hit) {
		vec2 uv = getScreenCoordByViewCoord((uniV * vec4(lastPoint,1.0f)).rgb);
		float testDepth = getLinearDepthOfViewCoord((uniV * vec4(lastPoint, 1.0f)).rgb);
		float sampleDepth = linearizeDepth(texture(uniDepthSampler, uv).x);
		hitColor = vec4(texture(uniAlbedoSpecSampler, uv).rgb, 1.0);
		hitColor.a = clamp(1.0 - pow(distance(uv, vec2(0.5)) * 2.0, 2.0), 0.0, 1.0);
	}
	return mix(color, hitColor.rgb, hitColor.a * schlick);
}
vec3 reflectEffect(vec3 color, vec3 normal, vec2 uv, vec3 worldPos, float attr) {
	if (attr == 1.0) {
		vec3 viewReflectRay = reflect(normalize(worldPos - viewPos), normal);
		vec2 uv2 = fragTextureCoord.st * vec2(1280, 768);
		float c = (uv2.x + uv2.y) * 0.25;
		float jitter = mod(c, 1.0);
		float schlick = 0.02 + 0.98 * pow(1.0 - dot(viewReflectRay, normal), 5.0);
		color = waterRayTarcing(worldPos + normal * (-worldPos.z / far * 0.2 + 0.05), viewReflectRay, color, 0, schlick);
	}
	return color;
}

void main() {
	// 从G缓冲中获取数据
	float specularStrength = 0.5;
	float roughness = texture(uniAlbedoSpecSampler, fragTextureCoord).a;
	vec3 fragPos = texture(uniPositionSampler, fragTextureCoord).rgb;
	vec3 originalColor = texture(uniAlbedoSpecSampler, fragTextureCoord).rgb;
	vec3 normal = texture(uniNormalSampler, fragTextureCoord).rgb;

	//part1
	vec3 ambient = light.ambient * lightColor;
	//part2
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = max(dot(lightDir, normal), 0.0f) * lightColor;
	//part3
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64); 
	vec3 specular = light.specular * spec * lightColor * vec3(1 - roughness);
	//part4
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//part5 shadow
	float bias = 0.0001;
	vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	float shadow = ShadowCalculation(bias, fragPosLightSpace);
	//part6 reflect
	vec4 color = vec4(attenuation * originalColor * (ambient + (diffuse + specular) * (1.0 - shadow)), 1.0f);
	//final
	//fColor = color;
	//fColor = vec4(vec3(ssr), 1.0f);
	//fColor = vec4(reflectEffect(texture(gDepthID, textureCoord).rgb, normal, textureCoord, fragPos, texture(gDepthID, textureCoord).a), 1.0f);
	fragColor = vec4(reflectEffect(color.rgb, normal, fragTextureCoord, fragPos, texture(uniDepthSampler, fragTextureCoord).a), 1.0f);
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