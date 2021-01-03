#version 450 core
layout (location = 0) in vec4 vertInPos;
layout (location = 1) in vec2 vertInTextureCoord; 

out vec2 fragInTextureCoord;

uniform mat4 uniM;

void main()
{
    gl_Position = uniM * vertInPos;
    fragInTextureCoord = vertInTextureCoord;
}