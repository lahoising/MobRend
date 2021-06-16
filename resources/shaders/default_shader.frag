#version 430 core
		
layout(location = 0) out vec4 finalFragColor;

layout(location = 0) in vec3 a_fragPos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
    int type;

    vec3 attenuation;
    float innerCutoff;
    float outerCutoff;
};


struct PhongMaterial
{
    vec3 diffuse;
    float diffuseMapStrength;

    vec3 specular;
    float specularMapStrength;

    float shininess;
};

layout(binding = 0) uniform sampler2D u_diffuseMap;
layout(binding = 1) uniform sampler2D u_specularMap;

layout(push_constant,std430) uniform Misc
{
    PhongMaterial phongMaterial;
    vec3 color;
    vec3 viewPos;
    Light pointLight;
    Light ambientLight;
} u_scene;

vec3 AmbientLight(Light light)
{
    return light.color * light.intensity * float(light.type == 0);
}

vec3 DiffuseLight(Light light, vec3 normal, vec3 fragmentPosition)
{
    vec3 lightDir = normalize(light.position - fragmentPosition) * float(light.type == 1 || light.type == 3);
    lightDir += normalize(-light.position) * float(light.type == 2); // position is direction if light is directional
    float diff = max(dot(normal, lightDir), 0.0);
    return light.color * light.intensity * diff;
}

vec3 SpecularLight(Light light, vec3 viewDir, vec3 fragPosition, vec3 normal, float shininess)
{
    vec3 lightDir = normalize(light.position - fragPosition) * float(light.type == 1 || light.type == 3);
    lightDir += normalize(-light.position) * float(light.type == 2);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return vec3(1.0f, 1.0f, 1.0f) * light.intensity * spec;
}

float Attenuation(Light light, vec3 fragPosition)
{
    float distance = length(light.position - fragPosition);
    float denominator = light.attenuation.x + 
                        (light.attenuation.y * distance) + 
                        (light.attenuation.z * distance * distance);
    return mix(1.0, 1.0 / denominator, light.type == 1); // dont use attenuation when light is not point
}

float WithinSpotlight(Light light, vec3 fragPosition)
{
    vec3 lightDir = normalize(light.position - fragPosition);
    float theta = dot(lightDir, normalize(-light.attenuation));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    return mix(1.0, intensity, light.type == 3);
}

void main(){
    vec4 diffuseColor = mix(
        vec4(u_scene.phongMaterial.diffuse, 1.0),
        texture(u_diffuseMap, a_texCoord),
        u_scene.phongMaterial.diffuseMapStrength);

    vec4 specularColor = mix(
        vec4(u_scene.phongMaterial.specular, 0.0),
        texture(u_specularMap, a_texCoord),
        u_scene.phongMaterial.specularMapStrength);

    float pointLightAttenuation = Attenuation(u_scene.pointLight, a_fragPos);
    float ambientAttenuation = Attenuation(u_scene.ambientLight, a_fragPos);

    float ambientSpotlight = WithinSpotlight(u_scene.ambientLight, a_fragPos);

    vec3 ambientLight = AmbientLight(u_scene.ambientLight) * ambientAttenuation * ambientSpotlight+ 
                        AmbientLight(u_scene.pointLight) * pointLightAttenuation;
    vec4 ambient = diffuseColor * vec4(ambientLight, 1.0);

    /// TODO: calculate diffuse in view space, not normal space
    vec3 norm = normalize(a_normal);
    vec3 diffuseLight = DiffuseLight(u_scene.ambientLight, norm, a_fragPos) * ambientAttenuation * ambientSpotlight + 
                        DiffuseLight(u_scene.pointLight, norm, a_fragPos) * pointLightAttenuation;
    vec4 diffuse = diffuseColor * vec4(diffuseLight, 1.0);

    vec3 viewDir = normalize(u_scene.viewPos - a_fragPos);
    vec3 specLight = 	SpecularLight(u_scene.ambientLight, viewDir, a_fragPos, norm, u_scene.phongMaterial.shininess) * ambientAttenuation * ambientSpotlight +
                        SpecularLight(u_scene.pointLight, viewDir, a_fragPos, norm, u_scene.phongMaterial.shininess) * pointLightAttenuation;
    vec4 specular = specularColor * vec4(specLight, 1.0);

    vec4 result = (ambient + diffuse + specular) * vec4(u_scene.color, 1.0);
    if(result.w < 0.1)
        discard;
    finalFragColor = result;
}