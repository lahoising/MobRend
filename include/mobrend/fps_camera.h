#ifndef _MR_FPS_CAMERA_H_
#define _MR_FPS_CAMERA_H_

#include <mobrend/camera.h>

namespace mr
{
    
class FPSCamera
{
public:
    FPSCamera() : camera(Camera()), yaw(0.0f), pitch(0.0f),
        movementSpeed(0.7f), sensitivity(0.01f) {}
    void Update();

public:
    Camera camera;
    float movementSpeed;
    float sensitivity;

private:
    float pitch, yaw;
};

} // namespace mr

#endif
