#version 450 core

layout(vertices = 4)out;

in vec3 fragPos[];
in vec2 textureCoord[];
out vec2 textureCoord_CTS_ETS[];
out vec3 fragPos_CTS_ETS[];

void main() {
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	fragPos_CTS_ETS[gl_InvocationID] = fragPos[gl_InvocationID];
	textureCoord_CTS_ETS[gl_InvocationID] = textureCoord[gl_InvocationID];

	float t = 64;

	// 下面设置细分曲面程度
	gl_TessLevelInner[0] = t;     // 内部划分3条垂直区域，即内部新增2列顶点
	gl_TessLevelInner[1] = t;     // 内部划分4条水平区域，即内部新增3行顶点

	gl_TessLevelOuter[0] = t;     // 左边2条线段
	gl_TessLevelOuter[1] = t;     // 下边3条线段
	gl_TessLevelOuter[2] = t;     // 右边4条线段
	gl_TessLevelOuter[3] = t;     // 上边5条线段
}