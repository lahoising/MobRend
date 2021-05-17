#include "mr_platform/mr_glfw_window.h"

namespace mr
{

GlfwWindow::GlfwWindow(Window::CreateParams createParams)
{
    WindowManager &windowManager = WindowManager::GetInstance();
    if(windowManager.GetWindowCount() == 0)
    {
        if(!glfwInit())
        {
            return;
        }
    }
    windowManager.AddWindow(this);
}

GlfwWindow::~GlfwWindow()
{
    WindowManager &windowManager = WindowManager::GetInstance();
    if(windowManager.GetWindowCount() == 1)
    {
        glfwTerminate();
    }
    windowManager.RemoveWindow(this);
}

} // namespace mr
