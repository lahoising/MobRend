#ifndef _MR_CAMERA_H_
#define _MR_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace mr
{
    
class Camera
{
public:
    enum Type : int
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    typedef struct
    {
        float fov;
        float aspectRatio;
        float near, far;
    } PerspectiveConfig;
    
    typedef struct
    {
        float size;
        float aspectRatio;
        float near, far;
    } OrthographicConfig;

    union Config
    {
        PerspectiveConfig perspective;
        OrthographicConfig ortho;
    };

public:
    Camera(Type type, Config config);

private:
    Config config;
    Type type;

    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 scale;
};

} // namespace mr

#endif