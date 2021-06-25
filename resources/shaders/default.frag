#version 430 core

layout(location = 0) out vec4 o_color;

layout(location = 0) in vec3 a_fragPos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

void main()
{
    o_color = vec4(0.4, 1.0, 0.4, 1.0);
}