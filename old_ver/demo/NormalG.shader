#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 100) out;

//debug
//layout(line_strip, max_vertices = 100) out;

//in vec2 textureCoord[];
in vec3 fragPos[];

//in vec3 fragPos_1[];
//in vec3 fragPosS;
//in float depth[];

//out vec2 textureCoord_GS_FS;
out vec3 fragPos_GS_FS;
out float depth_GS_FS;
//out vec3 normal_GS_FS;
out vec4 color_GS_FS;

uniform mat4 uniM;
uniform mat4 uniP;
uniform mat4 uniV;
uniform float uniTime;
//TODO : ันตน
uniform vec3 ObjPos;

float getLinearDepthOfViewCoord(vec4 viewCoord) {
	vec4 uniP = viewCoord;
	uniP = uniP * uniP;
	uniP /= uniP.w;
	return uniP.z;//linearizeDepth(uniP.z) / far;
}

#define BottomColor vec4(0.05, 0.4, 0.1, 1.0)
#define TopColor vec4(0.5, 0.7, 0.25, 1.0)

float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

mat4 AngleAxis4x4(float angle, vec3 axis)
{
	float c, s;
	s = asin(angle);
	c = acos(angle);
	//sincos(angle, s, c);

	float t = 1 - c;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	return mat4(
		t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0,
		t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0,
		t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0,
		0, 0, 0, 1);
}

void deal_with_debug();
void deal_with_grass();
void deal_with_wind_grass();

void main() {
	//deal_with_debug();
	//deal_with_grass();
	deal_with_wind_grass();
}

void deal_with_debug() {
	//float groundSize = 15.0f;
////fragPos[0] = vec3(groundSize, groundSize * 3 / 2, groundSize);
////fragPos[1] = vec3(groundSize, groundSize * 3 / 2, groundSize * 3);
////fragPos[2] = vec3(groundSize * 3, groundSize * 3 / 2, groundSize);

	vec4 point1;

	point1 = vec4(fragPos[0], 1.0f);
	gl_Position = uniP * uniV * point1;
	fragPos_GS_FS = point1.xyz;
	color_GS_FS = BottomColor;
	EmitVertex();
	point1 = vec4(fragPos[1], 1.0f);
	gl_Position = uniP * uniV * point1;
	fragPos_GS_FS = point1.xyz;
	color_GS_FS = TopColor;
	EmitVertex();
	point1 = vec4(fragPos[2], 1.0f);
	gl_Position = uniP * uniV * point1;
	fragPos_GS_FS = point1.xyz;
	color_GS_FS = TopColor / 2;
	EmitVertex();
	point1 = vec4(fragPos[0], 1.0f);
	gl_Position = uniP * uniV * point1;
	fragPos_GS_FS = point1.xyz;
	color_GS_FS = BottomColor;
	EmitVertex();
	EndPrimitive();

	////point1 = vec4(groundSize, groundSize * 3 / 2, groundSize, 1.0);
	////gl_Position = uniP * uniV * point1;
	////fragPos_GS_FS = point1.xyz;
	////color_GS_FS = BottomColor;
	////EmitVertex();
	////point1 = vec4(groundSize, groundSize * 3 / 2, groundSize * 3, 1.0);
	////gl_Position = uniP * uniV * point1;
	////fragPos_GS_FS = point1.xyz;
	////color_GS_FS = TopColor;
	////EmitVertex();
	////point1 = vec4(groundSize * 3, groundSize * 3 / 2, groundSize, 1.0);
	////gl_Position = uniP * uniV * point1;
	////fragPos_GS_FS = point1.xyz;
	////color_GS_FS = TopColor;
	////EmitVertex();
	////EndPrimitive();
}

void deal_with_grass() {
	//for (int n = 0; n < gl_in.length(); n++) {
		//gl_Position = gl_in[n].gl_Position;
		//textureCoord_GS_FS = textureCoord[n];
		//fragPos_GS_FS = fragPos[n];
		//depth_GS_FS = depth[n];
		//EmitVertex();
	//}
	//vec3 p0p1 = (gl_in[])
	//normal_GS_FS = 
	//vec4 point1 = (gl_in[0].gl_Position + gl_in[1].gl_Position) / 2;
	//vec4 point2 = (gl_in[1].gl_Position + gl_in[2].gl_Position) / 2;
	vec4 point1 = (vec4(fragPos[0], 1.0f) + vec4(fragPos[1], 1.0f)) / 2;
	vec4 point2 = (vec4(fragPos[1], 1.0f) + vec4(fragPos[2], 1.0f)) / 2;
	float temp = rand(point1.xz);
	int layers_num = 20;
	float grass_height = temp * 3;
	float layers_height = grass_height / layers_num / 2;
	float coe = 50 * temp;
	float test = 1;
	point1 += vec4(temp, -layers_height, temp, 0);
	point2 += vec4(temp, -layers_height, temp, 0);
	vec4 point3, point4, tempPoint;
	//vec4 point1, point2;
	//for (int gn = 0; gn < test; gn++) {
	//vec4 point1 = (vec4(fragPos[0], 1.0f) + vec4(fragPos[1], 1.0f)) / 2 - vec4(0, layers_height, 0, 0);
	//point1 = AngleAxis4x4(rand(point1.xy), vec3(0.0f, 1.0f, 0.0f)) * point1;
	//vec4 point2 = (vec4(fragPos[1], 1.0f) + vec4(fragPos[2], 1.0f)) / 2 - vec4(0, layers_height, 0, 0);
	//point2 = AngleAxis4x4(rand(point2.xy), vec3(0.0f, 1.0f, 0.0f)) * point2;

	for (int n = 0; n < layers_num; n++) {
		point3 = (point1 * coe + point2 * 1) / (1 + coe);
		gl_Position = uniP * uniV * point3;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * point3);
		fragPos_GS_FS = point3.xyz;
		color_GS_FS = mix(BottomColor, TopColor, 1.0f * n / 2 / layers_num);
		EmitVertex();
		point4 = (point1 * 1 + point2 * coe) / (1 + coe);
		gl_Position = uniP * uniV * point4;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * point4);
		fragPos_GS_FS = point4.xyz;
		color_GS_FS = mix(BottomColor, TopColor, 1.0f * n / 2 / layers_num);
		EmitVertex();
		coe *= 5;
		point1 = point3 + vec4(0.0f, layers_height, 0.0f, 0.0f);
		point2 = point4 + vec4(0.0f, layers_height, 0.0f, 0.0f);
	}
	coe = 100;
	for (int n = 0; n < layers_num; n++) {
		point3 = (point1 * coe + point2 * 1) / (1 + coe);
		gl_Position = uniP * uniV * point3;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * point3);
		fragPos_GS_FS = point3.xyz;
		color_GS_FS = mix(BottomColor, TopColor, 0.5f + 1.0f * n / 2 / layers_num);
		EmitVertex();
		point4 = (point1 * 1 + point2 * coe) / (1 + coe);
		gl_Position = uniP * uniV * point4;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * point4);
		fragPos_GS_FS = point4.xyz;
		color_GS_FS = mix(BottomColor, TopColor, 0.5f + 1.0f * n / 2 / layers_num);
		EmitVertex();
		coe *= 0.9;
		//coe = coe - 5;	
		point1 = point3 + vec4(0.0f, layers_height, 0.0f, 0.0f);
		point2 = point4 + vec4(0.0f, layers_height, 0.0f, 0.0f);
	}
	point3 = (point1 + point2) / 2 + vec4(0.0f, layers_height, 0.0f, 0.0f);
	gl_Position = uniP * uniV * point3;
	depth_GS_FS = getLinearDepthOfViewCoord(uniV * point3);
	fragPos_GS_FS = point3.xyz;
	color_GS_FS = mix(BottomColor, TopColor, 1);
	//EmitVertex();
	EndPrimitive();
}

void deal_with_wind_grass() {
	vec4 point1 = (vec4(fragPos[0], 1.0f) + vec4(fragPos[1], 1.0f)) / 2;
	vec4 point2 = (vec4(fragPos[1], 1.0f) + vec4(fragPos[2], 1.0f)) / 2;
	vec4 pointC = (point1 + point2) / 2;
	float temp = rand(point1.xz);
	int layers_num = 20;
	float grass_height = temp * 3;
	float layers_height = grass_height / layers_num / 2;
	float coe = 50 * temp;
	float test = 1;
	point1 += vec4(temp, -layers_height, temp, 0);
	point2 += vec4(temp, -layers_height, temp, 0);
	vec4 point3, point4, tempPoint;

	float heightCoe = 1.8;
	float speed = 5;
	float affectR = 5;
	float affectY = 10;
	float swiftCoe = 70;
	float windStrength = /*pow(abs(sin(uniTime/10 + pointC.x + pointC.z)), swiftCoe) **/ 30;
	for (int n = 0; n < layers_num; n++) {
		float part = 1.0f * n / 2 / layers_num;
		float shiftx;
		float shifty;
		float shiftz;
		float dist = sqrt(pow(ObjPos.x - pointC.x, 2) + pow(ObjPos.z - pointC.z, 2));
		if (dist <= affectR && ObjPos.y - pointC.y <= affectY) {
			shiftx = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) **/ uniTime + pow(pointC.z, 1) + pow(pointC.x, 1));
			shifty = -grass_height * (1 - dist / affectR);
			shiftz = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) * */uniTime + pow(pointC.z, 1) - pow(pointC.x, 1));
		}
		else {
			shiftx = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) **/ uniTime + pow(pointC.z, 1) + pow(pointC.x, 1));
			shifty = 0;
			shiftz = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) * */uniTime + pow(pointC.z, 1) - pow(pointC.x, 1));
		}

		point3 = (point1 * coe + point2 * 1) / (1 + coe);
		tempPoint = point3 + vec4(shiftx, shifty, shiftz, 0);
		gl_Position = uniP * uniV * tempPoint;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * tempPoint);
		fragPos_GS_FS = tempPoint.xyz;
		color_GS_FS = mix(BottomColor, TopColor, part);
		EmitVertex();
		point4 = (point1 * 1 + point2 * coe) / (1 + coe);
		tempPoint = point4 + vec4(shiftx, shifty, shiftz, 0);
		gl_Position = uniP * uniV * tempPoint;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * tempPoint);
		fragPos_GS_FS = tempPoint.xyz;
		color_GS_FS = mix(BottomColor, TopColor, part);
		EmitVertex();
		coe *= 5;
		point1 = point3 + vec4(0.0f, layers_height, 0.0f, 0.0f);
		point2 = point4 + vec4(0.0f, layers_height, 0.0f, 0.0f);
	}
	coe = 100;
	for (int n = 0; n < layers_num; n++) {
		float part = 0.5 + 1.0f * n / 2 / layers_num;
		float shiftx;
		float shifty;
		float shiftz;
		float dist = sqrt(pow(ObjPos.x - pointC.x, 2) + pow(ObjPos.z - pointC.z, 2));
		if (dist <= affectR && ObjPos.y - pointC.y <= affectY) {
			shiftx = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) **/ uniTime + pow(pointC.z, 1) + pow(pointC.x, 1));
			shifty = -grass_height * (1 - dist / affectR);
			shiftz = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) * */uniTime + pow(pointC.z, 1) - pow(pointC.x, 1));
		}
		else {
			shiftx = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) **/ uniTime + pow(pointC.z, 1) + pow(pointC.x, 1));
			shifty = 0;
			shiftz = windStrength * pow(part * layers_height, heightCoe) * sin(speed * /*pow(abs(sin((pointC.x + pointC.z))), swiftCoe) * */uniTime + pow(pointC.z, 1) - pow(pointC.x, 1));
		}

		point3 = (point1 * coe + point2 * 1) / (1 + coe);
		tempPoint = point3 + vec4(shiftx, shifty, shiftz, 0);
		gl_Position = uniP * uniV * tempPoint;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * tempPoint);
		fragPos_GS_FS = tempPoint.xyz;
		color_GS_FS = mix(BottomColor, TopColor, part);
		EmitVertex();
		point4 = (point1 * 1 + point2 * coe) / (1 + coe);
		tempPoint = point4 + vec4(shiftx, shifty, shiftz, 0);
		gl_Position = uniP * uniV * tempPoint;
		depth_GS_FS = getLinearDepthOfViewCoord(uniV * tempPoint);
		fragPos_GS_FS = tempPoint.xyz;
		color_GS_FS = mix(BottomColor, TopColor, part);
		EmitVertex();
		coe *= 0.9;
		point1 = point3 + vec4(0.0f, layers_height, 0.0f, 0.0f);
		point2 = point4 + vec4(0.0f, layers_height, 0.0f, 0.0f);
	}
	point3 = (point1 + point2) / 2 + vec4(0.0f, layers_height, 0.0f, 0.0f);
	gl_Position = uniP * uniV * point3;
	depth_GS_FS = getLinearDepthOfViewCoord(uniV * point3);
	fragPos_GS_FS = point3.xyz;
	color_GS_FS = mix(BottomColor, TopColor, 1);
	//EmitVertex();
	EndPrimitive();
}