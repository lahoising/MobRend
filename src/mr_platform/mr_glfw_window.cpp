#include "mr_platform/mr_glfw_window.h"

namespace mr
{
void OnWindowClose(GLFWwindow *window)
{
    Window *win = WindowManager::GetInstance().GetWindowByHandle(window);

    WindowCloseEvent windowCloseEvent = {};
    windowCloseEvent.window = win;

    InputEvent event = {};
    event.type = InputEventType::WINDOW_CLOSE;
    event.windowCloseEvent = windowCloseEvent;

    win->input.SubmitEvent(event);
}

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

    this->input = Input();
    glfwSetWindowCloseCallback(this->window, OnWindowClose);
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

void GlfwWindow::SwapBuffers()
{
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

void *GlfwWindow::GetHandle() { return this->window; }

} // namespace mr
