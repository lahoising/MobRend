#version 430 core

layout(location = 0) out vec4 o_color;

layout(location = 0) in vec3 a_fragPos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
    float type;
};

layout(push_constant,std140) uniform SceneLights 
{
    Light directional;
} u_sceneLights;

vec3 DiffuseLight(Light light, vec3 normal, vec3 fragmentPosition)
{
    vec3 lightDir = normalize(light.position - fragmentPosition) * float(light.type == 1.0 || light.type == 3.0);
    lightDir += normalize(-light.position) * float(light.type == 2.0); // position is direction if light is directional
    float diff = max(dot(normal, lightDir), 0.0);
    return light.color * light.intensity * diff;
}

void main()
{
    vec3 norm = normalize(a_normal);
    vec4 diffuse = vec4(DiffuseLight(u_sceneLights.directional, norm, a_fragPos), 1.0 );
    o_color = diffuse;
}