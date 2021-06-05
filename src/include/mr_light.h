#ifndef _MR_LIGHT_H_
#define _MR_LIGHT_H_

#include <glm/glm.hpp>

namespace mr
{

class Shader;
    
class Light
{
public:
    enum Type
    {
        AMBIENT = 0,
        POINT,
        DIRECTIONAL,
    };

public:
    Light(Type type, glm::vec3 color, float intensity);
    void Bind(Shader *shader, const char *name);

    glm::vec3 color;
    union
    {
        glm::vec3 position;
        glm::vec3 direction;
    };
    float intensity;
    Type type;
};

} // namespace mr

#endif