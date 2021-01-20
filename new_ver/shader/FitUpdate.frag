#version 450 core

layout(location = 0) out float heightMapNew;
//layout(location = 0) out vec4 fragColor;

uniform sampler2D heightMap;
uniform float radius;
uniform vec3 uniObjPos;
uniform vec3 uniObjVel;
uniform int rst;

in vec2 textureCoord;
in vec3 fragPos;

//TODO : vector go to 

void main() {
	float coe = 0.5;
	float base = 1;
	float baseLine = min((sqrt(textureCoord.x * coe) * sqrt(textureCoord.y * coe) * sqrt((1 - textureCoord.x) * coe) 
		* sqrt((1 - textureCoord.y) * coe)) * 16, base);
	if (rst == 1) {
		heightMapNew = baseLine;
	}
	else {

			float temp = texture(heightMap, textureCoord.xy).r;
			float R2 = pow(radius, 2);
			float D2 = pow(radius * 5, 2);
			float deltaX2 = pow(fragPos.x - uniObjPos.x, 2);
			float deltaZ2 = pow(fragPos.z - uniObjPos.z, 2);
			float delta1 = radius - sqrt(R2 - deltaX2 - deltaZ2);
			float delta2 = radius * 5 - sqrt(D2 - deltaX2 - deltaZ2);


			if (delta1 <= baseLine) {
				if (dot(uniObjVel.xz, fragPos.xz - uniObjPos.xz) >= 0) {
					heightMapNew = min(delta1, temp);
				}
				else heightMapNew = temp;
			}
			else if (delta2 <= baseLine) {
				if (dot(uniObjVel.xz, fragPos.xz - uniObjPos.xz) >= 0) {
					float linjie = radius * 5 - sqrt(D2 - R2 + pow(radius - baseLine, 2));
					float x = (delta2 - linjie) / (baseLine - linjie);
					if (temp < baseLine-0.1)
						heightMapNew = temp;
					else
						heightMapNew = max(temp, baseLine + 2 * log(6 * x + 1.5) / (6 * x + 1.5) - 0.54);
				}
				else heightMapNew = temp;
			}
			else {
				float volosity = min(abs(baseLine - temp), 0.999);
				volosity = -(volosity - 0) * (volosity - 1) * 0.001;
				heightMapNew = (baseLine - temp) * volosity + temp;
			}
	}


}