#version 430 core

layout(location = 0) in vec3 a_pos;

layout(push_constant, std430) uniform CameraConstants
{
    mat4 u_viewProjection;
    mat4 u_model;
    
} cameraConstants;

void main()
{
    gl_Position = cameraConstants.u_viewProjection * cameraConstants.u_model * vec4(a_pos, 1.0);
}