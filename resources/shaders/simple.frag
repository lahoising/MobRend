#version 430 core

layout(location = 0) in vec3 a_normal;
layout(location = 1) in vec2 a_uv;

layout(location = 0) out vec4 o_color;

// layout(binding = 1) uniform sampler2D u_texture;

void main()
{
    // o_color = texture(u_texture, a_uv);
    o_color = vec4(0.8, 0.4, 0.2, 1.0);
}