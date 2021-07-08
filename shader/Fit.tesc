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

	// ��������ϸ������̶�
	gl_TessLevelInner[0] = t;     // �ڲ�����3����ֱ���򣬼��ڲ�����2�ж���
	gl_TessLevelInner[1] = t;     // �ڲ�����4��ˮƽ���򣬼��ڲ�����3�ж���

	gl_TessLevelOuter[0] = t;     // ���2���߶�
	gl_TessLevelOuter[1] = t;     // �±�3���߶�
	gl_TessLevelOuter[2] = t;     // �ұ�4���߶�
	gl_TessLevelOuter[3] = t;     // �ϱ�5���߶�
}