#include <glm/gtx/quaternion.hpp>
#include "mr_camera.h"

namespace mr
{
    
Camera::Camera(Type type, Config config)
{
    this->rotation = glm::identity<glm::quat>();
    this->position = glm::vec3(0.0f, 0.0f, -5.0f);
    this->RecalculateViewMatrix();

    this->SetConfiguration(type, config);
}

void Camera::SetConfiguration(Type type, Config config)
{
    switch (type)
    {
    case Camera::Type::PERSPECTIVE:
        this->SetPerspective(config.perspective);
        break;
    
    case Camera::Type::ORTHOGRAPHIC:
        this->SetOrtho(config.ortho);
        break;

    default:
        break;
    }
}

void Camera::SetPerspective(PerspectiveConfig config)
{
    this->type = Camera::Type::PERSPECTIVE;
    this->projMatrix = glm::perspective(
        config.fov,
        config.aspectRatio, 
        config.near,
        config.far
    );
    RecalculateViewProjection();
}

void Camera::SetPosition(glm::vec3 position)
{
    this->position = position;
    this->RecalculateViewMatrix();
    this->RecalculateViewProjection();
}

void Camera::SetOrtho(OrthographicConfig config)
{
    this->type = Camera::Type::ORTHOGRAPHIC;

    float w = config.size;
    float h = config.size / config.aspectRatio;

    this->projMatrix = glm::ortho(
        -w,             w,
        -h,             h,
        config.near,    config.far
    );
    RecalculateViewProjection();
}

void Camera::RecalculateViewMatrix()
{
    const glm::vec3 fwd = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 target = fwd * this->rotation + this->position;
    
    this->viewMatrix = glm::lookAt(
        this->position,
        target,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

} // namespace mr
