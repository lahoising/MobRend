#ifndef _MR_GLFW_INPUT_H_
#define _MR_GLFW_INPUT_H_

#include "mr_input.h"
#include "mr_platform/mr_glfw_window.h"

namespace mr
{
    
class GlfwInput : public Input
{
public:
    GlfwInput(GlfwWindow *window);
    virtual void Clear() override;
    virtual void SwapState() override;
};

} // namespace mr

#endif