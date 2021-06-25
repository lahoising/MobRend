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

layout(push_constant,std140) uniform Scene 
{
    Light directional;
    vec3 viewPos;
} u_scene;

vec3 DiffuseLight(Light light, vec3 normal, vec3 fragmentPosition)
{
    vec3 lightDir = normalize(light.position - fragmentPosition) * float(light.type == 1.0 || light.type == 3.0);
    lightDir += normalize(-light.position) * float(light.type == 2.0); // position is direction if light is directional
    float diff = max(dot(normal, lightDir), 0.0);
    return light.color * light.intensity * diff;
}

vec3 SpecularLight(Light light, vec3 viewDir, vec3 fragPosition, vec3 normal, float shininess)
{
    vec3 lightDir = normalize(light.position - fragPosition) * float(light.type == 1.0 || light.type == 3.0);
    lightDir += normalize(-light.position) * float(light.type == 2.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return vec3(1.0, 1.0, 1.0) * light.intensity * spec;
}

void main()
{
    vec3 norm = normalize(a_normal);
    vec4 diffuse = vec4(DiffuseLight(u_scene.directional, norm, a_fragPos), 1.0 );

    vec3 viewDir = normalize(u_scene.viewPos - a_fragPos);
    vec4 specular = vec4(
        SpecularLight(
            u_scene.directional, 
            viewDir, 
            a_fragPos, 
            norm, 
            32.0),
        1.0);

    o_color = diffuse + specular;
}