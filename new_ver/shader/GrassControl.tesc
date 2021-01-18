#version 450 core

layout(vertices = 4)out;

in vec3 fragPos[];
out vec3 fragPos_CTS_ETS[];

void main() {
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	fragPos_CTS_ETS[gl_InvocationID] = fragPos[gl_InvocationID];

	float level = 64;
	// 下面设置细分曲面程度
	gl_TessLevelInner[0] = level;     // 内部划分3条垂直区域，即内部新增2列顶点
	gl_TessLevelInner[1] = level;     // 内部划分4条水平区域，即内部新增3行顶点

	gl_TessLevelOuter[0] = level;     // 左边2条线段
	gl_TessLevelOuter[1] = level;     // 下边3条线段
	gl_TessLevelOuter[2] = level;     // 右边4条线段
	gl_TessLevelOuter[3] = level;     // 上边5条线段
}