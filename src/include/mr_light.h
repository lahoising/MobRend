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
        POINT
    };

public:
    Light(Type type, glm::vec3 color, float intensity);
    void Bind(Shader *shader, const char *name);

    glm::vec3 color;
    glm::vec3 position;
    float intensity;
    Type type;
};

} // namespace mr

#endif