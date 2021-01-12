#version 450

layout(location = 0) out vec4 fColor;

in vec3 textureCoord;
uniform samplerCube skybox;

void main() {
	fColor = texture(skybox, textureCoord);
}