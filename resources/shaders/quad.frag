#version 430 core

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 a_texCoord;

layout(binding = 0) uniform sampler2D u_tex;

void main()
{
    out_color = texture(u_tex, a_texCoord);
}