#version 430 core

layout(location = 0) out vec4 o_color;

layout(location = 0) in vec3 a_fragPos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

struct PhongMaterial
{
    vec4 diffuse;
    float diffuseMapStrength;

    vec4 specular;
    float specularMapStrength;

    float shininess;
};

layout(binding = 1) uniform sampler2D u_diffuseMap;
layout(binding = 2) uniform sampler2D u_specularMap;

struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
    float type;
};

/// TODO: bind lights to UBOs
layout(push_constant,std140) uniform Scene 
{
    PhongMaterial material;
    Light directional;
    Light ambient;
    vec3 viewPos;
} u_scene;

vec3 AmbientLight(Light light)
{
    return light.color * light.intensity * float(light.type == 0.0);
}

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
    vec4 ambient = vec4(AmbientLight(u_scene.ambient), 1.0);

    vec3 norm = normalize(a_normal);
    vec4 diffuse = vec4(DiffuseLight(u_scene.directional, norm, a_fragPos), 1.0 );
    diffuse *= mix(
        u_scene.material.diffuse,
        texture(u_diffuseMap,a_texCoord),
        u_scene.material.diffuseMapStrength);

    vec3 viewDir = normalize(u_scene.viewPos - a_fragPos);
    vec4 specular = vec4(
        SpecularLight(
            u_scene.directional, 
            viewDir, 
            a_fragPos, 
            norm, 
            u_scene.material.shininess),
        1.0);
    specular *= mix(
        u_scene.material.specular,
        texture(u_specularMap,a_texCoord),
        u_scene.material.specularMapStrength);

    vec4 result = diffuse + specular + ambient;
    if(result.w < 0.1)
        discard;
    o_color = result;
}