#version 450 core

layout(early_fragment_tests)in;

layout(binding = 0, offset = 0) uniform atomic_uint index_counter;

layout(binding = 0, r32ui) uniform uimage2D head_pointer_image;
layout(binding = 1, rgba32ui) uniform uimageBuffer list_buffer;
layout(binding = 2, rgba32f) uniform imageBuffer list_buffer_worldPos;
layout(binding = 3, rgba32f) uniform imageBuffer list_buffer_normal;

layout(location = 0)out vec4 fColor;

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
	//vec4 frag_color = shadeFragment();
	uint index = atomicCounterIncrement(index_counter) + 1;
	//fColor = vec4(vec3(index * 0.1f), 1.0f);
	uint old_head = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), uint(index));
	//uint old_head = imageAtomicExchange(head_pointer_image, ivec2(0.5, 0.5), uint(1280 * 768));

	uvec4 item;
	item.x = old_head;
	item.y = packUnorm4x8(vec4(135 / 255.9, 206 / 255.9, 250 / 255.9, 0.5f));
	item.z = packUnorm4x8(vec4(normal, 1.0f));
	//item.w = floatBitsToUint(depth);
	item.w = floatBitsToUint(LinearizeDepth(gl_FragCoord.z) / uniFar);
	//item.w = floatBitsToUint(LinearizeDepth(depth) / uniFar);

	imageStore(list_buffer, int(index), item);
	imageStore(list_buffer_worldPos, int(index), vec4(worldPos, 1.0f));
	imageStore(list_buffer_normal, int(index), vec4(normal, 1.0f));
}