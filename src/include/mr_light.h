#ifndef _MR_LIGHT_H_
#define _MR_LIGHT_H_

#include <glm/glm.hpp>

namespace mr
{
    
class Light
{
public:
    enum Type
    {
        POINT
    };

public:
    Light(Type type, glm::vec3 color, float intensity);

    glm::vec3 color;
    glm::vec3 position;
    float intensity;
    Type type;
};

} // namespace mr

#endif