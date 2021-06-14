#version 430 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

layout(push_constant, std430) uniform CameraConstants
{
    mat4 viewProjection;
    mat4 model;
    
} u_cam;

void main()
{
    gl_Position = u_cam.viewProjection * u_cam.model * vec4(a_pos, 1.0);
}