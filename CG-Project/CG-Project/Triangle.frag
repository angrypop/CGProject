#version 450 core
layout (location = 0) out vec4 fragOutColor;
in vec4 fragInColor;
void main()
{
    fragOutColor = fragInColor;
}