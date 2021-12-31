#ifndef _MR_OBSERVER_CAMERA_H_
#define _MR_OBSERVER_CAMERA_H_

#include <glm/glm.hpp>
#include <mobrend/camera.h>

namespace mr
{
    
class ObserverCamera
{
public:
    ObserverCamera() 
        : camera(Camera()), focusPoint(glm::vec3(0.0f, 0.0f, 0.0f)) {}
    void Update();
    void SetFocusPoint(glm::vec3 point);

    Camera camera;

private:
    glm::vec3 focusPoint;
};

} // namespace mr

#endif
