#version 450 core
layout (location = 0) out vec4 fragOutColor;
in vec2 fragInTextureCoord;
uniform sampler2D uniColorSampler;

void main()
{
    fragOutColor = texture(uniColorSampler, fragInTextureCoord);
}