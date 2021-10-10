#include <glm/gtx/quaternion.hpp>
#include "mr_logger.h"
#include <mobrend/camera.h>

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
    this->projMatrix = glm::perspectiveLH(
        config.fov,
        config.aspectRatio, 
        config.near,
        config.far
    );
    RecalculateViewProjection();
}

void Camera::SetPosition(const glm::vec3 &position)
{
    this->position = position;
    this->RecalculateViewMatrix();
    this->RecalculateViewProjection();
}

void Camera::SetRotation(const glm::quat &rotation)
{
    this->rotation = rotation;
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
    glm::vec3 fwd = this->rotation * glm::vec3(0.0f, 0.0f, 1.0f);
    this->viewMatrix = glm::lookAtLH(
        this->position,
        fwd + this->position,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

} // namespace mr
