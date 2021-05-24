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

    void SetConfiguration(Type type, Config config);
    glm::mat4 GetViewProjection(){ return this->viewProjMat; }
    glm::vec3 GetPosition(){ return this->position; }
    void SetPosition(glm::vec3 position);

private:
    void SetPerspective(PerspectiveConfig config);
    void SetOrtho(OrthographicConfig config);

    
    void RecalculateViewMatrix();
    inline void RecalculateViewProjection(){ this->viewProjMat = this->projMatrix * this->viewMatrix; }


private:
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    glm::mat4 viewProjMat;

    glm::quat rotation;
    glm::vec3 position;
    Type type;
};

} // namespace mr

#endif