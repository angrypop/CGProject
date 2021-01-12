#version 450 core

//layout(triangles, equal_spacing, ccw) in;
layout(quads, equal_spacing, ccw) in;

in vec3 fragPos_CTS_ETS[];
in vec2 textureCoord_CTS_ETS[];
out vec3 fragPos_ET_GS;
out vec2 textureCoord_ET_GS;

float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void) {
	//for (int n = 0; n < gl_in.length; n++) {
	//	fragPos_1[n] = fragPos_CTS_ETS[n];
	//}

	float u = gl_TessCoord.x;
	float omu = 1 - u;
	float v = gl_TessCoord.y;
	float omv = 1 - v;
	//fragPos =
	//	(omu * omv * gl_in[0].gl_Position +
	//	u * omv * gl_in[1].gl_Position +
	//	u * v * gl_in[2].gl_Position +
	//	omu * v * gl_in[3].gl_Position).xyz;

	fragPos_ET_GS =
		(omu * omv * fragPos_CTS_ETS[1] +
			u * omv * fragPos_CTS_ETS[0] +
			u * v * fragPos_CTS_ETS[2] +
			omu * v * fragPos_CTS_ETS[3]).xyz;
	textureCoord_ET_GS = 
		(omu * omv * textureCoord_CTS_ETS[1] +
			u * omv * textureCoord_CTS_ETS[0] +
			u * v * textureCoord_CTS_ETS[2] +
			omu * v * textureCoord_CTS_ETS[3]);
	//fragPos.y = -fragPos.y;

	//fragPos.y += rand(fragPos.x,fragPos.z);
	//float temp = rand(fragPos.xz);
	//fragPos += vec3(0, temp, 0);

	//float groundSize = 15.0f;
	//fragPos_1[0] = vec3(groundSize, groundSize * 3 / 2, groundSize);
	//fragPos_1[1] = vec3(groundSize, groundSize * 3 / 2, groundSize * 3);
	//fragPos_1[2] = vec3(groundSize * 3, groundSize * 3 / 2, groundSize);
	//fragPosS = gl_TessCoord.x * fragPos_CTS_ETS[0] + gl_TessCoord.y * fragPos_CTS_ETS[1] + gl_TessCoord.z * fragPos_CTS_ETS[2];
}
