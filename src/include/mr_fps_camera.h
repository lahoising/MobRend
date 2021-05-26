#ifndef _MR_FPS_CAMERA_H_
#define _MR_FPS_CAMERA_H_

#include "mr_camera.h"

namespace mr
{
    
class FPSCamera
{
public:
    FPSCamera() : camera(Camera()) {}
    void Update();

public:
    Camera camera;
};

} // namespace mr

#endif