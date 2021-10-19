#ifndef _MR_GLFW_WINDOW_H_
#define _MR_GLFW_WINDOW_H_
#ifdef MOBREND_GLFW_WINDOW

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <mobrend/window.h>

namespace mr
{
    
class GlfwWindow : public Window
{
public:
    GlfwWindow(Window::CreateParams createParams);
    virtual ~GlfwWindow() override;
    virtual void SwapBuffers() override;
    virtual void PollEvents() override;
    virtual void SetCursorVisible(bool visible) override;
    virtual void *GetHandle() override { return this->window; }
    virtual uint32_t GetWidth()  override;
    virtual uint32_t GetHeight() override;
    virtual uint32_t GetFramebufferWidth() override;
    virtual uint32_t GetFramebufferHeight() override;
    GLFWwindow *GetWindow() { return this->window; }

private:
    GLFWwindow *window;
};

} // namespace mr

#endif
#endif
