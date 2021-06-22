#version 430 core

layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 Normal;
layout(location = 1) in vec3 Position;
layout(location = 2) in vec2 a_uv;

layout(push_constant,std430) uniform FragCam
{
    vec3 cameraPos;
} u_fragCam;

layout(binding = 0) uniform samplerCube u_skybox;

void main()
{
    vec3 I = normalize(Position - u_fragCam.cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(u_skybox, R).rgb, 1.0);
}