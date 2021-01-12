#version 450 core

layout(location = 0) out vec4 fColor;
in vec2 textureCoord;

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
uniform mat4 P;
uniform mat4 V;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepthID;
uniform sampler2D shadowMap;
uniform samplerCube skybox;

float ShadowCalculation(float bias, vec4 fragPosLightSpace)
{
	// 执行透视除法
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// 变换到[0,1]的范围
	projCoords = projCoords * 0.5 + 0.5;
	// 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// 取得当前片段在光源视角下的深度
	float currentDepth = projCoords.z;
	// 检查当前片段是否在阴影中
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	//float bias = 0.0005;//min(0.0001, currentDepth - closestDepth - 0.00000001);
	float shadow = currentDepth - closestDepth > bias ? 1.0 : 0.0;

	//float shadow = 0.0;
	//vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	//for (int x = -1; x <= 1; ++x)
	//{
	//	for (int y = -1; y <= 1; ++y)
	//	{
	//		float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
	//		shadow += currentDepth - bias >= pcfDepth ? 1.0 : 0.0;
	//	}
	//}
	//shadow /= 9.0;

	//fColor = vec4(vec3(currentDepth - closestDepth), 1.0f);
	//fColor = vec4(projCoords, 1.0f);

	//return
	//return closestDepth;
	//return currentDepth;
	if (projCoords.x >= 0 && projCoords.x <= 1 && projCoords.y >= 0 && projCoords.y <= 1)
		//fColor = vec4(projCoords, 1.0f);
		shadow = currentDepth - closestDepth > bias ? 1.0 : 0.0;
	else
		//fColor = vec4(0.5f);
		shadow = 0;


	return shadow;
	//return 0;
	//return currentDepth - closestDepth;
}

#define BISEARCH(SEARCHPOINT, DIRVEC, SIGN) DIRVEC *= 0.5; \
                    SEARCHPOINT+= DIRVEC * SIGN; \
                    uv = getScreenCoordByViewCoord(SEARCHPOINT); \
                    sampleDepth = linearizeDepth(texture(gDepthID, uv).x); \
                    testDepth = getLinearDepthOfViewCoord(SEARCHPOINT); \
                    SIGN = sign(sampleDepth - testDepth);
float linearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}
float getLinearDepthOfViewCoord(vec3 viewCoord) {
	vec4 p = vec4(viewCoord, 1.0);
	p = P * p;
	p /= p.w;
	return linearizeDepth(p.z) / far;
}
vec2 getScreenCoordByViewCoord(vec3 viewCoord) {
	vec4 p = vec4(viewCoord, 1.0);
	p = P * p;
	p /= p.w;
	if (p.z < -1 || p.z>1) return vec2(-1.0);
	p = p * 0.5 + 0.5;
	return p.st;
}
vec3 waterRayTarcing(vec3 startPoint, vec3 direction, vec3 color, float jitter, float schlick) {
	//return direction * 0.5 + 0.5;

	const float stepBase = 1;
	vec3 testPoint = startPoint;
	vec3 lastPoint = testPoint;
	direction *= stepBase;
	bool hit = false;
	vec4 hitColor = vec4(0.0);

	for (int i = 0; i < 160; i++) {
		//testPoint += direction * pow(float(i + 1 + jitter), 1.46);
		testPoint += direction;
		vec2 uv = getScreenCoordByViewCoord((V * vec4(testPoint, 1.0f)).rgb);
		if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
			hit = false;
			break;
		}
		float sampleDepth = texture(gDepthID, uv).r;//texture2DLod(depthtex0, uv, 0.0).x;
		//sampleDepth = linearizeDepth(sampleDepth);
		float testDepth = getLinearDepthOfViewCoord((V * vec4(testPoint, 1.0f)).rgb);
		if (sampleDepth > 0.1 && sampleDepth < testDepth && testDepth - sampleDepth < (1.0 / 1024.0) * (1.0 + testDepth * 200 + float(i))) {
			//hit = true;
			vec3 finalPoint = lastPoint;
			float _sign = 1.0;
			vec3 tempDirection = testPoint - lastPoint;
			//BISEARCH(finalPoint, tempDirection, _sign);
			//BISEARCH(finalPoint, tempDirection, _sign);
			//BISEARCH(finalPoint, tempDirection, _sign);
			//BISEARCH(finalPoint, tempDirection, _sign);
			//BISEARCH(finalPoint, tempDirection, _sign);
			//BISEARCH(finalPoint, tempDirection, _sign);
			//BISEARCH(finalPoint, tempDirection, _sign);
			//BISEARCH(finalPoint, tempDirection, _sign);
			uv = getScreenCoordByViewCoord((V * vec4(finalPoint, 1.0f)).rgb);
			hitColor = vec4(texture(gAlbedoSpec, uv).rgb, 1.0);
			//hitColor = vec4(texture2DLod(gcolor,uv,0.0).rgb,1.0);
			//hitColor = vec4(bloom(texture2DLod(gcolor, uv, 0.0).rgb, uv) * 0.5, 1.0);
			hitColor.a = clamp(1.0 - pow(distance(uv, vec2(0.5)) * 2.0, 2.0), 0.0, 1.0);
			break;
		}
		lastPoint = testPoint;
	}
	if (!hit) {

		vec2 uv = getScreenCoordByViewCoord((V * vec4(lastPoint,1.0f)).rgb);
		float testDepth = getLinearDepthOfViewCoord((V * vec4(lastPoint, 1.0f)).rgb);
		float sampleDepth = linearizeDepth(texture(gDepthID, uv).x);
		//if (testDepth - sampleDepth < 0.5) {
			//hitColor = vec4(texture2DLod(gcolor, uv, 0.0).rgb, 1.0);
			hitColor = vec4(texture(gAlbedoSpec, uv).rgb, 1.0);
			hitColor.a = clamp(1.0 - pow(distance(uv, vec2(0.5)) * 2.0, 2.0), 0.0, 1.0);
		//}
		//return color;
		//return texture(skybox, normalize(direction)).rgb;
	}
	//if (hit && startPoint.z < lastPoint.z)

	//return 100 * vec3(texture(gDepthID, getScreenCoordByViewCoord((V * vec4(startPoint, 1.0f)).rgb)) - texture(gDepthID, getScreenCoordByViewCoord((V * vec4(testPoint, 1.0f)).rgb)));
	//return normalize(direction * 0.5 + 0.5);
	//return direction * 0.5 + 0.5;
	//return hit ? vec3(1.0f) : vec3(0.0f);
	//return hitColor.rgb * hitColor.a;
	////vec3 pureColor = vec3(164.0 / 255.0, 211.0 / 255.0, 238.0 / 255.0);
	//return mix(pureColor, hitColor.rgb, hitColor.a * schlick);
	//return mix(color, vec3(0.5f, 0.5f, 0.5f), 1 * schlick);
	//return vec3(-(V * vec4(testPoint, 1.0f)).z);
	//return vec3((V * vec4(startPoint, 1.0f)).z);
	//return vec3(-getLinearDepthOfViewCoord((V * vec4(startPoint, 1.0f)).xyz) + getLinearDepthOfViewCoord((V * vec4(lastPoint, 1.0f)).xyz));
	//return hit && startPoint.z > lastPoint.z ? mix(color, hitColor.rgb, hitColor.a * schlick) : color;
	return mix(color, hitColor.rgb, hitColor.a * schlick);
	//return lastPoint;
}
vec3 reflectEffect(vec3 color, vec3 normal, vec2 uv, vec3 worldPos, float attr) {
	if (attr == 1.0) {
		//vec3 normal = normalDecode(texture2D(gnormal, texcoord.st).rg);
		vec3 viewReflectRay = reflect(normalize(worldPos - viewPos), normal);
		//viewReflectRay = normalize(viewReflectRay);
		vec2 uv2 = textureCoord.st * vec2(1280, 768);
		float c = (uv2.x + uv2.y) * 0.25;
		float jitter = mod(c, 1.0);
		float schlick = 0.02 + 0.98 * pow(1.0 - dot(viewReflectRay, normal), 5.0);
		//color = waterRayTarcing(viewPos + normal * (-viewPos.z / far * 0.2 + 0.05), viewReflectRay, color, jitter, schlick);
		color = waterRayTarcing(worldPos + normal * (-worldPos.z / far * 0.2 + 0.05), viewReflectRay, color, 0, schlick);
	}
	return color;
}

vec3 aces_tonemap(vec3 color) {
	mat3 m1 = mat3(
		0.59719, 0.07600, 0.02840,
		0.35458, 0.90834, 0.13383,
		0.04823, 0.01566, 0.83777
	);
	mat3 m2 = mat3(
		1.60475, -0.10208, -0.00327,
		-0.53108, 1.10813, -0.07276,
		-0.07367, -0.00605, 1.07602
	);
	vec3 v = m1 * color;
	vec3 a = v * (v + 0.0245786) - 0.000090537;
	vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	return pow(clamp(m2 * (a / b), 0.0, 1.0), vec3(1.0 / 2.2));
}

//vec2 getScreenCoordByViewCoord(vec3 viewCoord) {
//	vec4 p = vec4(viewCoord, 1.0);
//	p = P * p;
//	p /= p.w;
//	if (p.z < -1 || p.z > 1)
//		return vec2(-1.0);
//	p = p * 0.5f + 0.5f;
//	//p = p * 2 - 1;
//	return p.st;
//}
//
//float linearizeDepth(float depth) {
//	float z = depth * 2.0 - 1.0; // back to NDC 
//	return (2.0 * near * far) / (far + near - z * (far - near));
//}
//
//float getLinearDepthOfViewCoord(vec3 viewCoord) {
//	vec4 p = vec4(viewCoord, 1.0);
//	p = P * p;
//	p /= p.w;
//	return linearizeDepth(p.z) / far;
//}
//vec3 waterRayTarcing(vec3 startPoint, vec3 direction, vec3 color, float schlick) {
//	const float stepBase = 0.05;
//	vec3 testPoint = startPoint;
//	direction *= stepBase;
//	bool hit = false;
//	vec4 hitColor = vec4(0.0);
//	for (int i = 0; i < 100; i++) {
//		testPoint += direction;
//		vec2 uv = getScreenCoordByViewCoord((V * vec4(testPoint, 1.0f)).xyz);
//		float sampleDepth = texture(gDepthID, uv).x;
//		//sampleDepth = linearizeDepth(sampleDepth);
//		float testDepth = getLinearDepthOfViewCoord((V * vec4(testPoint, 1.0f)).xyz);
//		if (sampleDepth < testDepth) {
//			hit = true;
//			hitColor = vec4(texture(gAlbedoSpec, uv).rgb, 1.0);
//			break;
//		}
//	}
//	if (!hit) return color;
//	//return normalize(direction * 0.5 + 0.5);
//	//if (abs(texture(gDepthID, getScreenCoordByViewCoord((V * vec4(startPoint, 1.0f)).xyz)).r - getLinearDepthOfViewCoord((V * vec4(startPoint, 1.0f)).xyz)) < 0.01) return vec3(1.0f); else return vec3(0.0f);
//	//return vec3(0.01 / (abs(texture(gDepthID, getScreenCoordByViewCoord((V * vec4(startPoint, 1.0f)).xyz)).r - getLinearDepthOfViewCoord((V * vec4(startPoint, 1.0f)).xyz)) + 0.01));
//	//return texture(gDepthID, getScreenCoordByViewCoord((V * vec4(startPoint, 1.0f)).xyz)).rrr;
//	return vec3(getLinearDepthOfViewCoord((V * vec4(testPoint, 1.0f)).xyz));
//	//return mix(color, hitColor.rgb, hitColor.a * schlick * 2);
//	//return texture(gAlbedoSpec, getScreenCoordByViewCoord((V * vec4(startPoint, 1.0f)).xyz)).rgb;
//	//return vec3(getScreenCoordByViewCoord((V * vec4(startPoint, 1.0f)).xyz).rg, 0.0f);
//	//return (V * vec4(testPoint, 1.0f)).rgb;
//}
//vec3 reflectEffect(vec3 color, vec3 normal, vec2 uv, vec3 worldPos, float attr) {
//	if (attr == 1.0) {
//		vec3 viewRefRay = reflect(normalize(worldPos - viewPos), normal);
//		float schlick = 0.02 + 0.98 * pow(1.0 - dot(viewRefRay, normal), 5.0);
//		color = waterRayTarcing(worldPos + normal * (-worldPos.z / far * 0.2 + 0.05), viewRefRay, color, schlick);
//	}
//	return color;
//}

#define START_H float(200.0)
#define END_H float(1600.0)
#define FOG_COLOR vec3(0.95)

vec3 GenFogColor(vec4 worldPos, float depthView, vec3 sceneColor) {
	float worldH = worldPos.y / worldPos.w;
	float heightFactor = smoothstep(START_H, END_H, worldH);
	float fogFactor = exp2(-0.0000005 * depthView * depthView * log(2));

	fogFactor = mix(fogFactor, 1.0, heightFactor);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	return mix(FOG_COLOR, sceneColor, fogFactor);
}


void main() {
	// 从G缓冲中获取数据
	float specularStrength = 0.5;
	float roughness = texture(gAlbedoSpec, textureCoord).a;
	vec3 fragPos = texture(gPosition, textureCoord).rgb;
	vec3 originalColor = texture(gAlbedoSpec, textureCoord).rgb;
	vec3 normal = texture(gNormal, textureCoord).rgb;

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
	//spec = 0;
	vec3 specular = light.specular * spec * lightColor * vec3(1 - roughness);
	//part4
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//float isLight = dot(lightDir, normal) * dot(viewDir, normal) < 0 ? 0 : 1;
	//part5 shadow
	//float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.001);  
	float bias = 0.0001;
	vec4 FragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	float shadow = ShadowCalculation(bias, FragPosLightSpace);
	//shadow = 0;
	//part6 reflect
	vec4 color = vec4(attenuation * originalColor * (ambient + (diffuse + specular) * (1.0 - shadow)), 1.0f);
	//vec3 ssr = reflectEffect(color.rgb, normal, textureCoord, fragPos, 1.0f);
	//ssr = 0.01 / (abs(ssr - texture(gDepthID, textureCoord).r) + 0.01);
	//final
	//fColor = color;
	//fColor = vec4(vec3(ssr), 1.0f);
	//fColor = vec4(reflectEffect(texture(gDepthID, textureCoord).rgb, normal, textureCoord, fragPos, texture(gDepthID, textureCoord).a), 1.0f);
	//fColor = vec4(reflectEffect(color.rgb, normal, textureCoord, fragPos, texture(gDepthID, textureCoord).a), 1.0f);
	//fColor = vec4(GenFogColor(vec4(fragPos,1.0), texture(gDepthID, textureCoord).r, fColor.rgb), 1);

	fColor = vec4(reflectEffect(color.rgb, normal, textureCoord, fragPos, texture(gDepthID, textureCoord).a), 1.0f);
	fColor.rgb = vec3(1.0) - exp(-fColor.rgb * 2.5);

	//fColor.rgb = pow(fColor.rgb, vec3(1.0 / 2.2));

	//fColor = vec4(aces_tonemap(fColor.rgb), 1.0f);
	//fColor = vec4(attenuation * originalColor * (ambient + (diffuse + specular) * (1.0 - shadow)), 1.0f);
	//fColor = vec4(originalColor, 1.0);
	//fColor = vec4(texture(gDepthID, textureCoord).rgb/1000, 1.0f);
	//fColor = vec4(normal, 1.0);
	//fColor = vec4(reflect(normalize(fragPos - viewPos), normal) * 0.5 + 0.5, 1.0f);
	//fColor = vec4(shadow );
	//fColor = vec4((P * V * vec4(fragPos, 1.0f)).zzzz/100);
	//vec4 temp = FragPosLightSpace / FragPosLightSpace.w * 0.5 + 0.5;
	//fColor = texture(shadowMap,temp.xy).rrrr;
}