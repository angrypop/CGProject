#version 450 core

layout(location = 0) out vec4 fColor;
in vec2 textureCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepthID;

void main() {
	fColor = vec4(texture(gDepthID, textureCoord).rgb, 1.0);
}