#include "mr_light.h"
#include "mr_shader.h"

namespace mr
{

/// ======== AMBIENT LIGHT ===========
AmbientLight::AmbientLight(glm::vec3 color, float intensity)
    : Light(Light::Type::AMBIENT, color, intensity) {}

void AmbientLight::Bind(Shader *shader, const char *name)
{
    char attributeNameBuffer[256] = {};

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.position", name);
    shader->UploadVec3(attributeNameBuffer, this->position);
    
    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.color", name);
    shader->UploadVec3(attributeNameBuffer, this->color);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.intensity", name);
    shader->UploadFloat(attributeNameBuffer, this->intensity);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.type", name);
    shader->UploadInt(attributeNameBuffer, (int)this->GetType());
}

/// =================== POINT LIGHT ==============
PointLight::PointLight(glm::vec3 color, float intensity)
    : Light(Light::Type::POINT, color, intensity), attenuation(glm::vec3(1.0f, 0.22f, 0.20f)){}

void PointLight::Bind(Shader *shader, const char *name)
{
    char attributeNameBuffer[256] = {};

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.position", name);
    shader->UploadVec3(attributeNameBuffer, this->position);
    
    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.color", name);
    shader->UploadVec3(attributeNameBuffer, this->color);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.intensity", name);
    shader->UploadFloat(attributeNameBuffer, this->intensity);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.type", name);
    shader->UploadInt(attributeNameBuffer, (int)this->GetType());
    
    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.attenuation", name);
    shader->UploadVec3(attributeNameBuffer, this->attenuation);
}

/// ================ DIRECTIONAL LIGHT ================
DirectionalLight::DirectionalLight(glm::vec3 color, float intensity)
    : Light(Light::Type::DIRECTIONAL, color, intensity), direction(0.0f, 0.0f, 1.0f) {}

void DirectionalLight::Bind(Shader *shader, const char *name)
{
    char attributeNameBuffer[256] = {};

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.position", name);
    shader->UploadVec3(attributeNameBuffer, this->direction);
    
    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.color", name);
    shader->UploadVec3(attributeNameBuffer, this->color);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.intensity", name);
    shader->UploadFloat(attributeNameBuffer, this->intensity);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.type", name);
    shader->UploadInt(attributeNameBuffer, (int)this->GetType());
}

} // namespace mr
