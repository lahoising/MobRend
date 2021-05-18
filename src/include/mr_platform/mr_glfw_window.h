#ifndef _MR_GLFW_WINDOW_H_
#define _MR_GLFW_WINDOW_H_

#ifdef USE_GLFW
#include "GLFW/glfw3.h"
#include "mr_window.h"

namespace mr
{
    
class GlfwWindow : public Window
{
public:
    GlfwWindow(Window::CreateParams createParams);
    virtual ~GlfwWindow() override;
    virtual void SwapBuffers() override;

    GLFWwindow *GetWindow() { return this->window; }

private:
    GLFWwindow *window;
};

} // namespace mr

#endif
#endif