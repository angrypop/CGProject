#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 100) out;

//debug
//layout(line_strip, max_vertices = 100) out;

//in vec2 textureCoord[];
in vec3 fragPos_ET_GS[];
in vec2 textureCoord_ET_GS[];
//in vec3 fragPos_1[];
//in vec3 fragPosS;
in float depth[];

//out vec2 textureCoord_GS_FS;
out vec3 fragPos;
out vec2 textureCoord;
out float depth_GS_FS;
out vec3 normal_GS_FS;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;
//uniform float time; 
//uniform vec3 ObjPos;
uniform sampler2D heightMap;

float getLinearDepthOfViewCoord(vec4 viewCoord) {
	vec4 p = viewCoord;
	p = P * p;
	p /= p.w;
	return p.z;//linearizeDepth(p.z) / far;
}

float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
	vec3 point[3];
	float radius = 5;
	float coe = 0.5;

	//TODO 根据texture设定高度
	for (int i = 0; i < 3; i++) {
		point[i] = fragPos_ET_GS[i];
		//point[i].y += texture(heightMap, textureCoord_ET_GS[i]).x;
		//point[i].y += (
		//	texture(heightMap, textureCoord_ET_GS[i]).x +
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x, textureCoord_ET_GS[i].y + 0.001)).x +
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x, textureCoord_ET_GS[i].y - 0.001)).x +
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x + 0.001, textureCoord_ET_GS[i].y)).x +
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x - 0.001, textureCoord_ET_GS[i].y)).x + 
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x + 0.001, textureCoord_ET_GS[i].y + 0.001)).x +
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x - 0.001, textureCoord_ET_GS[i].y + 0.001)).x +
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x + 0.001, textureCoord_ET_GS[i].y - 0.001)).x +
		//	texture(heightMap, vec2(textureCoord_ET_GS[i].x - 0.001, textureCoord_ET_GS[i].y - 0.001)).x) / 9;

		float displace = 0.0;
		vec2 texelSize = 1.0 / textureSize(heightMap, 0);
		for (int x = -3; x <= 3; ++x) {
			for (int y = -3; y <= 3; ++y) {
				float pcfDepth = texture(heightMap, textureCoord_ET_GS[i] + vec2(x, y) * texelSize).r;
				displace += pcfDepth;
			}
		}
		point[i].y += displace / 49;

		//point[i].y += min((sqrt(textureCoord_ET_GS[i].x * coe) * sqrt(textureCoord_ET_GS[i].y * coe) * sqrt((1 - textureCoord_ET_GS[i].x) * coe) * sqrt((1 - textureCoord_ET_GS[i].y) * coe)) * 16, 1);
		//point[i].y = min(radius + fragPos_ET_GS[i].y - sqrt(pow(radius, 2) - pow(point[i].x - 0, 2) - pow(point[i].z - 30, 2)), point[i].y);
	}
	/*point2 = fragPos_ET_GS[1];
	point2.y += min((sqrt(point2.x * coe) * sqrt(point2.y * coe) * sqrt((1 - point2.x) * coe) * sqrt((1 - point2.y) * coe)) * 16, 1);
	point2.y = min(radius + temp - sqrt(pow(radius, 2) - pow(point2.x - 0, 2) - pow(point2.z - 30, 2)), point2.y);
	point3 = fragPos_ET_GS[2];
	point3.y += min((sqrt(point3.x * coe) * sqrt(point3.y * coe) * sqrt((1 - point3.x) * coe) * sqrt((1 - point3.y) * coe)) * 16, 1);
	point3.y = min(radius + temp - sqrt(pow(radius, 2) - pow(point3.x - 0, 2) - pow(point3.z - 30, 2)), point3.y);*/

	//vec3 line1 = point1 - point2;
	//vec3 line2 = point2 - point3;
	//normal_GS_FS = normalize(-cross(line1, line2));
	vec3 line1 = point[0] - point[1];
	vec3 line2 = point[1] - point[2];
	normal_GS_FS = normalize(cross(line1, line2));

	for (int i = 0; i < 3; i++) {
		//point1 = vec4(fragPos_ET_GS[i], 1.0f); 
		gl_Position = P * V * vec4(point[i], 1.0f);
		fragPos = point[i].xyz;
		depth_GS_FS = getLinearDepthOfViewCoord(V * vec4(point[i], 1.0f));
		textureCoord = textureCoord_ET_GS[i];
		EmitVertex();
	}
	EndPrimitive();

	//point1 = vec4(fragPos_ET_GS[0], 1.0f);
	//gl_Position = P * V * point1; 
	//fragPos = point1.xyz; fragPos.y = 0;
	//textureCoord = textureCoord_ET_GS[0];
	//EmitVertex();
	//point1 = vec4(fragPos_ET_GS[1], 1.0f);
	//gl_Position = P * V * point1;
	//fragPos = point1.xyz; fragPos.y = 0;
	//textureCoord = textureCoord_ET_GS[1];
	//EmitVertex();
	//point1 = vec4(fragPos_ET_GS[2], 1.0f);
	//gl_Position = P * V * point1;
	//fragPos = point1.xyz; fragPos.y = 0;
	//textureCoord = textureCoord_ET_GS[2];
	//EmitVertex();
	//point1 = vec4(fragPos_ET_GS[0], 1.0f);
	//gl_Position = P * V * point1;
	//fragPos = point1.xyz; fragPos.y = 0;
	//textureCoord = textureCoord_ET_GS[0];
	//EmitVertex();
	//point1 = vec4(fragPos_ET_GS[1], 1.0f);
	//gl_Position = P * V * point1;
	//fragPos = point1.xyz; fragPos.y = 0;
	//textureCoord = textureCoord_ET_GS[1];
	//EmitVertex();
	//point1 = vec4(fragPos_ET_GS[2], 1.0f);
	//gl_Position = P * V * point1;
	//fragPos = point1.xyz; fragPos.y = 0;
	//textureCoord = textureCoord_ET_GS[2];
	//EmitVertex(); EndPrimitive();
}