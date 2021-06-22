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
    Camera() : Camera(
        Type::PERSPECTIVE, 
        {{60.0f, 16.0f/9.0f, 0.1f, 100.0f}}){}

    void SetConfiguration(Type type, Config config);
    const glm::mat4 &GetViewProjection() const { return this->viewProjMat; }

    const glm::mat4 &GetViewMatrix() const { return this->viewMatrix; }
    const glm::mat4 &GetProjectionMatrix() const { return this->projMatrix; }
    
    const glm::vec3 &GetPosition() const { return this->position; }
    void      SetPosition(const glm::vec3 &position);
    const glm::quat &GetRotation() const { return this->rotation; }
    void      SetRotation(const glm::quat &rotation);

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