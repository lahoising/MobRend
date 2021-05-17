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

    this->window = glfwCreateWindow(
        createParams.width,
        createParams.height,
        createParams.windowName,
        NULL, NULL
    );

    if(!this->window)
    {
        return;
    }

    glfwMakeContextCurrent(this->window);
}

GlfwWindow::~GlfwWindow()
{
    WindowManager &windowManager = WindowManager::GetInstance();
    if(this->window)
    {
        glfwDestroyWindow(this->window);
    }

    if(windowManager.GetWindowCount() == 1)
    {
        glfwTerminate();
    }
    windowManager.RemoveWindow(this);
}

} // namespace mr
