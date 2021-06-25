#version 430 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texCoord;

layout(location = 0) out vec2 out_texCoord;

layout(binding = 0) uniform CamMatrices
{
    mat4 u_viewProjection;
};

layout(push_constant,std430) uniform ModelMatrices
{
    mat4 model;
} u_model;

void main()
{
    gl_Position = u_viewProjection * u_model.model * vec4(a_pos, 1.0);
    // gl_Position = u_model.model * vec4(a_pos, 1.0);
    out_texCoord = a_texCoord;
}