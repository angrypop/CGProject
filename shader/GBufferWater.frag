#version 450 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec4 gDepthID;

in vec3 worldPos;
in vec3 normal;
in float depth;

uniform float uniNear;
uniform float uniFar;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * uniNear * uniFar) / (uniFar + uniNear - z * (uniFar - uniNear));
}
void main() {
	// �洢��һ��G���������е�Ƭ��λ������
	gPosition = worldPos;
	// ͬ���洢��ÿ����Ƭ�η��ߵ�G������
	gNormal = normal;
	// ���������ÿ����Ƭ����ɫ
	gAlbedoSpec.rgb = vec3(135 / 255.9, 206 / 255.9, 250 / 255.9);
	// �洢����ǿ�ȵ�gAlbedoSpec��alpha����
	gAlbedoSpec.a = 0;

	gDepthID.rgb = vec3(LinearizeDepth(gl_FragCoord.z) / uniFar);
	//gDepthID.rgb = vec3(gl_FragCoord.z);
	//gDepthID.rgb = vec3(getLinearDepthOfViewCoord(worldPos));
	//gDepthID.rgb = vec3(LinearizeDepth(depth) / uniFar);
	gDepthID.a = 1;
}