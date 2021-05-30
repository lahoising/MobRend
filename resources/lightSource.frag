#version 410 core

out vec4 finalFragColor;
uniform vec3 u_lightColor;

void main()
{
    finalFragColor = vec4(u_lightColor, 1.0);
}