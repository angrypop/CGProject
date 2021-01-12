#version 450 core

layout(location = 0) out vec4 fColor;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Light light;

uniform vec3 lightColor;
uniform vec3 viewPos;

//uniform mat4 normalMat;
uniform mat3 TBN;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D roughnessTexture;

in vec3 fragPos;
in vec2 textureCoord;
//in vec4 fColorFromv;

void main() {
	float specularStrength = 0.5;

	vec3 originalColor = texture(colorTexture, textureCoord).rgb;
	vec3 normal = texture(normalTexture, textureCoord).rgb;
	normal = normalize(normal * 2 - vec3(1.0, 1.0, 1.0));
	normal = normalize(TBN * normal);
	//vec3 normal = vec3(0, 1, 0);

	//part1
	vec3 ambient = light.ambient * lightColor;
	//part2
	vec3 lightDir = normalize(light.position-fragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = max(dot(lightDir,normal),0.0f) * lightColor; 
	//part3
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = light.specular * spec * lightColor * (1 - texture(roughnessTexture, textureCoord)).rrr;
	//part4
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	fColor = vec4(attenuation*originalColor*(ambient+diffuse+specular),1.0f);
	//fColor = vec4(normal, 1.0f);
	//fColor = vec4(originalColor ,1);
}