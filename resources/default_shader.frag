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
    PhongMaterial u_phongMaterial;
    vec3 u_color;
    vec3 u_viewPos;
    Light u_pointLight;
    Light u_ambientLight;
};

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
    vec3 diffuseColor = mix(
        u_phongMaterial.diffuse,
        vec3(texture(u_diffuseMap, a_texCoord)),
        u_phongMaterial.diffuseMapStrength);

    vec3 specularColor = mix(
        u_phongMaterial.specular,
        vec3(texture(u_specularMap, a_texCoord)),
        u_phongMaterial.specularMapStrength);

    float pointLightAttenuation = Attenuation(u_pointLight, a_fragPos);
    float ambientAttenuation = Attenuation(u_ambientLight, a_fragPos);

    float ambientSpotlight = WithinSpotlight(u_ambientLight, a_fragPos);

    vec3 ambientLight = AmbientLight(u_ambientLight) * ambientAttenuation * ambientSpotlight+ 
                        AmbientLight(u_pointLight) * pointLightAttenuation;
    vec3 ambient = diffuseColor * ambientLight;

    vec3 norm = normalize(a_normal);
    vec3 diffuseLight = DiffuseLight(u_ambientLight, norm, a_fragPos) * ambientAttenuation * ambientSpotlight + 
                        DiffuseLight(u_pointLight, norm, a_fragPos) * pointLightAttenuation;
    vec3 diffuse = diffuseColor * diffuseLight;

    vec3 viewDir = normalize(u_viewPos - a_fragPos);
    vec3 specLight = 	SpecularLight(u_ambientLight, viewDir, a_fragPos, norm, u_phongMaterial.shininess) * ambientAttenuation * ambientSpotlight +
                        SpecularLight(u_pointLight, viewDir, a_fragPos, norm, u_phongMaterial.shininess) * pointLightAttenuation;
    vec3 specular = specularColor * specLight;

    vec3 result = (ambient + diffuse + specular) * u_color;
    // vec3 result = norm;
    finalFragColor = vec4(result, 1.0);
}