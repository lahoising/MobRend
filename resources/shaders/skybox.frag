#version 430 core

layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec3 TexCoords;

layout(binding =0) uniform samplerCube u_skybox;

void main()
{
    FragColor = texture(u_skybox, TexCoords);
}