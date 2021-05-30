#include "mr_light.h"

namespace mr
{

Light::Light(Type type, glm::vec3 color, float intensity)
    : color(color), position(glm::vec3()), intensity(intensity), type(type)
{
    
}

} // namespace mr
