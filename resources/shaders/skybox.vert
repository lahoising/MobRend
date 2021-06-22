#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 0) out vec3 TexCoords;

layout(push_constant,std430) uniform Cam
{
    mat4 viewProjection;
} u_cam;

void main()
{
    TexCoords = aPos;
    gl_Position = u_cam.viewProjection * vec4(aPos, 1.0);
}