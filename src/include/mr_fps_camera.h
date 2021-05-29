#ifndef _MR_FPS_CAMERA_H_
#define _MR_FPS_CAMERA_H_

#include "mr_camera.h"

namespace mr
{
    
class FPSCamera
{
public:
    FPSCamera() : camera(Camera()), yaw(0.0f), pitch(0.0f) {}
    void Update();

public:
    Camera camera;

private:
    float pitch, yaw;
};

} // namespace mr

#endif