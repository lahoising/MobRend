#version 430 core

layout(location = 0) in vec3 a_pos;

layout(push_constant, std430) uniform PushConstants
{
    mat4 u_viewProjection;
    mat4 u_model;
} registers;

void main()
{
    gl_Position = registers.u_viewProjection * registers.u_model * vec4(a_pos, 1.0);
}