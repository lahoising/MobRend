#version 430 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

layout(location = 0) out vec3 o_normal;
layout(location = 1) out vec2 o_uv;

layout(binding = 0,std140) uniform CamMatrices
{
    mat4 u_viewProjection;
    mat3 u_normalMat;
};

layout(push_constant,std140) uniform ModelMatrix
{
    mat4 u_model;
};

void main()
{
    o_normal = u_normalMat * a_normal;
    o_uv = a_uv;
    vec3 pos = vec3(u_model * vec4(a_pos, 1.0));
    gl_Position = u_viewProjection * vec4(pos, 1.0);
}