#version 430 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_texCoord;

layout(location = 0) out vec2 out_texCoord;

void main()
{
    gl_Position = vec4(a_pos, 0.0, 1.0);
    out_texCoord = a_texCoord;
}