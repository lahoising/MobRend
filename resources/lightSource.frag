#version 430 core

layout(location = 0) out vec4 finalFragColor;
layout(push_constant, std430) uniform PushConstants {
    vec3 u_lightColor;
} registers;

void main()
{
    finalFragColor = vec4(registers.u_lightColor, 1.0);
}