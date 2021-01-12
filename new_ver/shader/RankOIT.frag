#version 450 core

//layout(location = 0) out vec3 _gPosition;
//layout(location = 1) out vec3 _gNormal;
//layout(location = 3) out vec4 _gDepthID;
layout(location = 4) out vec4 _gTransColor;
in vec2 textureCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepthID;
uniform sampler2D gTransColor;

layout(binding = 0, r32ui) uniform uimage2D head_pointer_image;
layout(binding = 1, rgba32ui) uniform uimageBuffer list_buffer;
layout(binding = 2, rgba32f) uniform imageBuffer list_buffer_worldPos;
layout(binding = 3, rgba32f) uniform imageBuffer list_buffer_normal;


#define MAX_FRAGMENTS 15
uvec4 fragments[MAX_FRAGMENTS];
vec4 worldPos[MAX_FRAGMENTS];
vec4 normal[MAX_FRAGMENTS];

int build_local_fragment_list(float limit) {
	uint current;
	int frag_count = 0;

	//current = texelFetch(head_pointer_image, ivec2(gl_FragCoord.xy), 0);
	current = imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).x;

	while (current != 0x0 && current != 0xFFFFFFFF && frag_count < MAX_FRAGMENTS) {
		uvec4 item = imageLoad(list_buffer, int(current));
		vec4 worldPos_temp = imageLoad(list_buffer_worldPos, int(current));
		vec4 normal_temp = imageLoad(list_buffer_normal, int(current));
		if (uintBitsToFloat(item.w) <= limit || limit == 0) {
			fragments[frag_count] = item;
			worldPos[frag_count] = worldPos_temp;
			normal[frag_count] = normal_temp;
			frag_count++;
		}
		current = item.x;
	}
	return frag_count;
}
void sort_fragment_list(int frag_count) {

	int i;
	int j;
	for (i = 0; i < frag_count; i++) {
		for (j = i + 1; j < frag_count; j++) {
			float depth_i = uintBitsToFloat(fragments[i].w);
			float depth_j = uintBitsToFloat(fragments[j].w);
			if (depth_i > depth_j) {
				uvec4 temp = fragments[i];
				fragments[i] = fragments[j];
				fragments[j] = temp;
				vec4 temp_ = worldPos[i];
				worldPos[i] = worldPos[j];
				worldPos[j] = temp_;
				temp_ = normal[i];
				normal[i] = normal[j];
				normal[j] = temp_;
			}
		}
	}
}

vec4 calculate_final_color(int frag_count, vec3 base_color) {
	vec3 final_color = base_color;
	float blender = 1;
	//for (int i = 0; i < frag_count; i++) {
	//	vec4 color = unpackUnorm4x8(fragments[i].y);
	//	final_color = mix(final_color, color.xyz, blender * color.a);
	//	blender *= color.a;
	//}
	for (int i = frag_count - 1; i >= 0; i--) {
		vec4 color = unpackUnorm4x8(fragments[i].y);
		final_color = mix(final_color, color.xyz, /*blender * */color.a);
		blender *= color.a;
	}
	return vec4(final_color, blender);
}

void main() {
	int frag_count;
	frag_count = build_local_fragment_list(texture(gDepthID, textureCoord).r);
	sort_fragment_list(frag_count);
	_gTransColor = calculate_final_color(frag_count, texture(gAlbedoSpec, textureCoord).rgb);
	vec3 color = _gTransColor.rgb;
	//_gAlbedoSpec = vec4(color, 1.0f);
	//_gAlbedoSpec = vec4(frag_count * 1.0f / MAX_FRAGMENTS);
	//if (frag_count != 0) {
	//	_gPosition = worldPos[0].xyz;
	//	_gNormal = normal[0].xyz;
	//	_gDepthID = vec4(vec3(uintBitsToFloat(fragments[0].w)), normal[0].w);
	//	if (normal[0].w == 1) { //water
	//		//linear fog
	//		float deltaDepth = texture(gDepthID, textureCoord).r - uintBitsToFloat(fragments[0].w);
	//		color = mix(color, vec3(165 / 255.9, 236 / 255.9, 250 / 255.9), 1 - 1 / exp(deltaDepth));
	//		_gTransColor = vec4(color, _gTransColor.a);
	//		//TODO
	//	}
	//}
	//else {
	//	_gPosition = texture(gPosition, textureCoord).xyz;
	//	_gNormal = texture(gNormal, textureCoord).xyz;
	//	_gDepthID = texture(gDepthID, textureCoord).rgba;
	//}
}
