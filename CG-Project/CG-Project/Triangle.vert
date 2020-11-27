#version 450 core
layout (location = 0) in vec4 vertInPos;
layout (location = 1) in vec4 vertInColor;

uniform mat4 uniRotate;

out vec4 fragInColor;
void main()
{
    gl_Position = uniRotate * vertInPos;
    fragInColor = vertInColor;
}