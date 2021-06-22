#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 a_uv;

layout(location = 0) out vec3 Normal;
layout(location = 1) out vec3 Position;
layout(location = 2) out vec2 o_uv;

layout(push_constant,std430) uniform Cam
{
    mat4 model;
    mat4 viewProjection;
    mat3 normalMat;
} u_cam;

void main()
{
    Normal = u_cam.normalMat * aNormal;
    Position = vec3(u_cam.model * vec4(aPos, 1.0));
    o_uv = a_uv;
    gl_Position = u_cam.viewProjection * vec4(Position, 1.0);
}