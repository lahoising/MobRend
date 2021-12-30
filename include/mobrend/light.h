#ifndef _MR_LIGHT_H_
#define _MR_LIGHT_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace mr
{

class Shader;

/// ================== BASE LIGHT ===================   
class Light
{
public:
    enum Type : int
    {
        AMBIENT = 0,
        POINT,
        DIRECTIONAL,
        SPOTLIGHT,
    };

public:
    virtual ~Light() = 0;
    virtual void Bind(Shader *shader, const char *name) = 0;
    Type GetType() { return this->type; }

protected:
    Light(Type type, glm::vec3 color, float intensity)
        : color(color), position(glm::vec3()), intensity(intensity), type(type) {}

public:
    glm::vec3 color;
    glm::vec3 position;

    float intensity;

protected:
    Type type;
};

/// ================ AMBIENT LIGHT =============
class AmbientLight : public Light
{
public:
    AmbientLight(glm::vec3 color, float intensity);
    virtual void Bind(Shader *shader, const char *name) override;
};

/// ================== POINT LIGHT ===================
class PointLight : public Light
{
public:
    PointLight(glm::vec3 color, float intensity);
    virtual void Bind(Shader *shader, const char *name) override;

public:
    glm::vec3 attenuation;
};

/// ====================== DIRECTIONAL LIGHT ================
class DirectionalLight : public Light
{
public:
    DirectionalLight(glm::vec3 color, float intensity);
    virtual void Bind(Shader *shader, const char *name) override;

public:
    glm::vec3 direction;
};

/// =========== SPOTLIGHT =============
class Spotlight : public Light
{
public:
    Spotlight(glm::vec3 color, float intensity);
    virtual void Bind(Shader *shader, const char *name) override;
    
public:
    glm::quat rotation;
    float innerCutoff;
    float outerCutoff;
};


} // namespace mr

#endif