#version 450 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertTextureCoord; 

out vec3 fragPosition;
out vec2 fragTextureCoord;

uniform mat4 uniM;
uniform mat4 uniV;
uniform mat4 uniP;

void main()
{
    gl_Position = uniP * uniV * uniM * vec4(vertPos, 1.0);
    fragPosition = (uniM * vec4(vertPos, 1.0)).xyz;
    fragTextureCoord = vertTextureCoord;
}