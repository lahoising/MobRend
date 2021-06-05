#include "mr_light.h"
#include "mr_shader.h"

namespace mr
{

Light::Light(Type type, glm::vec3 color, float intensity)
    : color(color), position(glm::vec3()), intensity(intensity), type(type)
{
    
}

void Light::Bind(Shader *shader, const char *name)
{
    char attributeNameBuffer[256] = {};

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.position", name);
    shader->UploadVec3(attributeNameBuffer, this->position);    // since position and direction occupy the same space, no need to bind direction specifically
    
    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.color", name);
    shader->UploadVec3(attributeNameBuffer, this->color);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.intensity", name);
    shader->UploadFloat(attributeNameBuffer, this->intensity);

    snprintf(attributeNameBuffer, sizeof(attributeNameBuffer), "%s.type", name);
    shader->UploadInt(attributeNameBuffer, (int)this->type);
}

} // namespace mr
