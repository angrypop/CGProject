#version 450 core

layout(location = 0) out vec4 fColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepthID;

in vec2 textureCoord;
layout(binding = 0, r32ui) uniform uimage2D head_pointer_image;
layout(binding = 1, rgba32ui) uniform uimageBuffer list_buffer;

#define MAX_FRAGMENTS 15
uvec4 fragments[MAX_FRAGMENTS];

int build_local_fragment_list(float limit) {
	uint current;
	int frag_count = 0;

	//current = texelFetch(head_pointer_image, ivec2(gl_FragCoord.xy), 0);
	current = imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).x;
	//current = 0xFFFFFFFF;
	//debug
	//frag_color = vec4(vec3(limit), 1.0f);
	//frag_color = vec4(vec3(uintBitsToFloat(imageLoad(list_buffer, int(current)).w)), 1.0f);

	while (current != 0x0 && current != 0xFFFFFFFF && frag_count < MAX_FRAGMENTS) {
		uvec4 item = imageLoad(list_buffer, int(current));
		if (uintBitsToFloat(item.w) <= limit || limit == 0) {
			fragments[frag_count] = item;
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
			}
		}
	}
}

vec3 calculate_final_color(int frag_count, vec3 base_color) {
	vec3 final_color = base_color;
	for (int i = 0; i < frag_count; i++) {
		final_color = mix(final_color, vec3(135 / 255.9, 206 / 255.9, 250 / 255.9), 0.8);
	}
	return final_color;
}

void main() {
	int frag_count;
	frag_count = build_local_fragment_list(texture(gDepthID, textureCoord).r);
	sort_fragment_list(frag_count);
	vec3 color = calculate_final_color(frag_count, texture(gAlbedoSpec, textureCoord).rgb);
	//if (frag_count != 0) {
	//	frag_color = vec4(1);
	//}
	//else {
	//	frag_color = vec4(0);
	//}

	//fColor = vec4(color, 1.0f);
	//fColor = vec4(imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).xxx * 1.0f / 1280 / 768, 1.0f);
	//fColor = vec4(vec3(frag_count * 1.0f / MAX_FRAGMENTS), 1.0f);
	//fColor = vec4(vec3(texture(gDepthID, textureCoord).r - uintBitsToFloat(imageLoad(list_buffer, int(imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).x)).w)), 1.0f);
	//fColor = vec4(vec3(uintBitsToFloat(imageLoad(list_buffer, int(imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).x)).w)), 1.0f);
	fColor = vec4(vec3(texture(gDepthID, textureCoord).r), 1.0f);
	//fColor = vec4(vec3(imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).x)*100, 1.0f);
	//fColor = vec4(vec3(imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).x) * 100, 1.0f);
}
