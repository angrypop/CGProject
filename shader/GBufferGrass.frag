#version 450 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec4 gDepthID;

in vec3 fragPos_GS_FS;
//in vec2 textureCoord_GS_FS;
in vec4 color_GS_FS;
in float depth_GS_FS;

//uniform sampler2D colorTexture;
//uniform sampler2D normalTexture;
//uniform sampler2D roughnessTexture;
//uniform mat3 TBN;

uniform float uniNear;
uniform float uniFar;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * uniNear * uniFar) / (uniFar + uniNear - z * (uniFar - uniNear));
}

void main() {
	// �洢��һ��G���������е�Ƭ��λ������
	gPosition = fragPos_GS_FS;
	// ͬ���洢��ÿ����Ƭ�η��ߵ�G������
	gNormal = vec3(0.0f, 1.0f, 0.0f);
	//gNormal = normalize(TBN * normalize(texture(normalTexture, textureCoord).rgb * 2 - 1));
	// ���������ÿ����Ƭ����ɫ
	//gAlbedoSpec.rgb = texture(colorTexture, textureCoord).rgb;
	gAlbedoSpec.rgb = color_GS_FS.rgb;
	//gAlbedoSpec.rgb = vec3(0.5f);
	// �洢����ǿ�ȵ�gAlbedoSpec��alpha����
	gAlbedoSpec.a = 0.9f;
	//gAlbedoSpec.a = texture(roughnessTexture, textureCoord).r;

	gDepthID.rgb = vec3(LinearizeDepth(gl_FragCoord.z) / uniFar);
	//gDepthID.rgb = vec3(LinearizeDepth(depth_GS_FS) / uniFar);
	//gDepthID.a = 2.0f;
}