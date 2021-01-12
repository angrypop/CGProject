#version 450 core

layout(location = 0) out float heightMapNew;
//layout(location = 0) out vec4 fragColor;

uniform sampler2D heightMap;
uniform float radius;
uniform vec3 ObjPos;
uniform vec3 ObjVel;
uniform int rst;

in vec2 textureCoord;
in vec3 fragPos;

//TODO : vector go to 

void main() {
	//if (rst == 1) {
	float coe = 0.5;
	float base = 1;
	float baseLine = min((sqrt(textureCoord.x * coe) * sqrt(textureCoord.y * coe) * sqrt((1 - textureCoord.x) * coe) * sqrt((1 - textureCoord.y) * coe)) * 16, base);
	//heightMapNew = vec3(0.5f);
	//fragColor = vec4(1.0f);

	//heightMapNew = min((sqrt(textureCoord.x * coe) * sqrt(textureCoord.y * coe) * sqrt((1 - textureCoord.x) * coe) * sqrt((1 - textureCoord.y) * coe)) * 16, 1);
	//heightMapNew = min(radius - sqrt(pow(radius, 2) - pow(fragPos.x - ObjPos.x, 2) - pow(fragPos.z - ObjPos.z, 2)), heightMapNew);

	if (rst == 1) {
		//gl_FragDepth = min((sqrt(textureCoord.x * coe) * sqrt(textureCoord.y * coe) * sqrt((1 - textureCoord.x) * coe) * sqrt((1 - textureCoord.y) * coe)) * 16, 1);
		heightMapNew = baseLine;
		//heightMapNew = min(radius - sqrt(pow(radius, 2) - pow(fragPos.x - ObjPos.x, 2) - pow(fragPos.z - ObjPos.z, 2)), heightMapNew);
	}
	else {

			float temp = texture(heightMap, textureCoord.xy).r;

			//float radiusPlane = sqrt(pow(radius)

			float R2 = pow(radius, 2);
			float D2 = pow(radius * 5, 2);
			float deltaX2 = pow(fragPos.x - ObjPos.x, 2);
			float deltaZ2 = pow(fragPos.z - ObjPos.z, 2);

			float delta1 = radius - sqrt(R2 - deltaX2 - deltaZ2);
			float delta2 = radius * 5 - sqrt(D2 - deltaX2 - deltaZ2);
			//float delta2 = delta1 / 4;
			//heightMapNew = delta1;

			if (delta1 <= baseLine) {
				if (dot(ObjVel.xz, fragPos.xz - ObjPos.xz) >= 0) {
					heightMapNew = min(delta1, temp);
				}
				else heightMapNew = temp;
				//heightMapNew = delta1;
				//heightMapNew = min(radius - sqrt(pow(radius, 2) - pow(fragPos.x - ObjPos.x, 2) - pow(fragPos.z - ObjPos.z, 2)), temp);
			}
			else if (delta2 <= baseLine) {
				if (dot(ObjVel.xz, fragPos.xz - ObjPos.xz) >= 0) {
					float linjie = radius * 5 - sqrt(D2 - R2 + pow(radius - baseLine, 2));
					//float x = (delta2-baseLine/4)*4 + 1;
					//float x = (delta2 - linjie) + 1;
					//heightMapNew = baseLine + log(x) / x ;//min(radius - sqrt(R2 - deltaX2 - deltaZ2), temp);
					float x = (delta2 - linjie) / (baseLine - linjie);
					//heightMapNew = baseLine + x;
					if (temp < baseLine-0.1)
						heightMapNew = temp;
					else
						heightMapNew = max(temp, baseLine + 2 * log(6 * x + 1.5) / (6 * x + 1.5) - 0.54);
				}
				else heightMapNew = temp;
			}
			else {
				//heightMapNew = (baseLine - temp) / 10 + temp;
				float volosity = min(abs(baseLine - temp), 0.999);
				volosity = -(volosity - 0) * (volosity - 1) * 0.001;
				heightMapNew = (baseLine - temp) * volosity + temp;
				//heightMapNew = temp;
			}
	}
	//heightMapNew.x = min(heightMapNew.y, texture(heightMap, textureCoord).r);
	//gl_FragDepth = 1;
	//gl_FragDepth = 0.5f;
	//}

}