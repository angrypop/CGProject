#version 450 core
layout (location = 1) out vec3 fragPositionOut;
layout (location = 0) out vec3 fragColor;

in vec3 fragPosition;
in vec2 fragTextureCoord;

uniform sampler2D uniColorSampler;

void main()
{
    fragPositionOut = fragPosition;
    fragColor = texture(uniColorSampler, fragTextureCoord).rgb;
}