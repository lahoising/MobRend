#version 430 core

layout(location = 0) out vec4 finalFragColor;
layout(push_constant, std430) uniform LightConstants
{
    vec3 u_lightColor;
} lightConstants;

void main()
{
    finalFragColor = vec4(lightConstants.u_lightColor, 1.0);
}