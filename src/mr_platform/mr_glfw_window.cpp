#include <unordered_map>

#include "mr_platform/mr_glfw_window.h"
#include "mr_application.h"

namespace mr
{

static std::unordered_map<GLFWwindow*,GlfwWindow*> windowMap;

static void OnGlfwWindowClose(GLFWwindow *win)
{
    // auto &windowManager = WindowManager::GetInstance();
    // GlfwWindow *window = windowManager.GetWindow(win);
    GlfwWindow *window = windowMap[win];
    
    WindowCloseEvent windowCloseEvent = {};
    windowCloseEvent.window = (Window*)window;

    InputEvent event = {};
    event.type = InputEventType::WINDOW_CLOSE;
    event.event.windowClose = windowCloseEvent;

    window->input.SubmitEvent(event);
}

GlfwWindow::GlfwWindow(Window::CreateParams createParams)
{
    auto &windowManager = WindowManager::GetInstance();
    if(windowManager.GetWindowCount() == 0)
    {
        if(!glfwInit())
        {
            return;
        }
    }

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
    windowManager.AddWindow(this);
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
    auto &windowManager = WindowManager::GetInstance();
    
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
