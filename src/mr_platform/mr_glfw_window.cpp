#include <unordered_map>

#include "mr_platform/mr_glfw_window.h"
#include "mr_application.h"

namespace mr
{

static std::unordered_map<GLFWwindow*,GlfwWindow*> windowMap;

static void OnGlfwWindowClose(GLFWwindow *win)
{
    GlfwWindow *window = windowMap[win];
    window->Close();
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

    windowMap[this->window] = this;
    glfwMakeContextCurrent(this->window);

    this->input = Input();
    glfwSetWindowCloseCallback(this->window, OnGlfwWindowClose);
}

void GlfwWindow::Close()
{
    Application::GetInstance().Close();
}

GlfwWindow::~GlfwWindow()
{
    WindowManager &windowManager = WindowManager::GetInstance();
    
    windowManager.RemoveWindow(this);
    if(this->window)
    {
        windowMap.erase(this->window);
        glfwDestroyWindow(this->window);
        this->window = nullptr;
    }

    if(windowManager.GetWindowCount() == 0)
    {
        glfwTerminate();
        printf("window bye!\n");
    }
}

void GlfwWindow::SwapBuffers()
{
    glfwSwapBuffers(this->window);
    this->input.SwapState();
    glfwPollEvents();
}

} // namespace mr
