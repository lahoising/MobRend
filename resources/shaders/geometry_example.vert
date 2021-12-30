#version 430 core
layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec3 a_color;

layout(location = 0) out vec3 o_color;

void main()
{
    gl_Position = vec4(a_pos, 0.0, 1.0);
    o_color = a_color;
}
