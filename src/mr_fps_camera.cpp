#include <GLFW/glfw3.h>
#include "mr_logger.h"
#include "mr_fps_camera.h"
#include "mr_application.h"
#include "mr_window.h"
#include "mr_input.h"

namespace mr
{
    
void FPSCamera::Update()
{
    Input &input = Application::GetInstance().GetMainWindow()->input;

    glm::vec3 position = this->camera.GetPosition();
    glm::quat rotation = this->camera.GetRotation();
    glm::vec3 worldFwd = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 camFwd = worldFwd * rotation;
    glm::vec3 camRight = worldRight * rotation;

    const float CAM_MOVEMENT_SPEED = 0.1f;
    glm::vec3 deltaMove = glm::vec3(0.0f, 0.0f, 0.0f);
    deltaMove += camRight * (float)(input.IsKeyPressed(GLFW_KEY_D) - input.IsKeyPressed(GLFW_KEY_A));
    deltaMove += camFwd * (float)(input.IsKeyPressed(GLFW_KEY_W) - input.IsKeyPressed(GLFW_KEY_S));
    deltaMove *= CAM_MOVEMENT_SPEED;

    this->camera.SetPosition(position + deltaMove);
}

} // namespace mr
