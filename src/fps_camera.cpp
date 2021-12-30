#include <GLFW/glfw3.h>
#include <mr_logger.h>
#include <mobrend/fps_camera.h>
#include <mobrend/application.h>
#include <mobrend/window.h>
#include <mobrend/input.h>

namespace mr
{
    
void FPSCamera::Update()
{
    Input &input = Application::GetInstance().GetMainWindow()->input;

    glm::vec3 position = this->camera.GetPosition();
    glm::quat rotation = this->camera.GetRotation();
    glm::vec3 worldFwd = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camFwd = rotation * worldFwd;
    glm::vec3 camRight = rotation * worldRight;
    glm::vec3 camUp = rotation * worldUp;

    glm::vec3 deltaMove = glm::vec3(0.0f, 0.0f, 0.0f);
    deltaMove += camRight * (float)(input.IsKeyPressed(KEY_D) - input.IsKeyPressed(KEY_A));
    deltaMove += camFwd * (float)(input.IsKeyPressed(KEY_W) - input.IsKeyPressed(KEY_S));
    deltaMove += camUp * (float)(input.IsKeyPressed(KEY_R) - input.IsKeyPressed(KEY_F));
    deltaMove *= this->movementSpeed;

    glm::vec2 mouseDelta = input.GetMouseDelta() * this->sensitivity;
    
    yaw += glm::radians(mouseDelta.x);
    while(yaw > 180.0f) yaw -= 360.0f;
    while(yaw < -180.0f) yaw += 360.0f;

    pitch += glm::radians(mouseDelta.y);
    pitch = glm::clamp(
        pitch,
        glm::radians(-89.9f),
        glm::radians(89.9f)
    );
    rotation = glm::quat( glm::vec3(pitch, yaw, 0.0f) );

    this->camera.SetPosition(position + deltaMove);
    this->camera.SetRotation(rotation);
}

} // namespace mr
