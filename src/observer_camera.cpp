#include <glm/gtx/quaternion.hpp>
#include "mr_observer_camera.h"
#include "mr_application.h"
#include "mr_window.h"
#include "mr_input.h"

namespace mr
{
    
void ObserverCamera::Update()
{
    Input &input = Application::GetInstance().GetMainWindow()->input;

    // glm::vec3 position = this->camera.GetPosition();
    // glm::quat rotation = this->camera.GetRotation();

    // glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f) * rotation;
    
    // glm::vec3 direction = this->focusPoint - position;
    // glm::vec3 normalizedDirection = glm::normalize(direction);

    // glm::quat newOrientation = glm::quatLookAt(normalizedDirection, camUp);
    // this->camera.SetRotation(newOrientation);
}

void ObserverCamera::SetFocusPoint(glm::vec3 point)
{
    this->focusPoint = point;
}

} // namespace mr
