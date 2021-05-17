#include <functional>

#include "mr_platform/mr_glfw_window.h"
#include "mr_application.h"

template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
   template <typename... Args> 
   static Ret callback(Args... args) {                    
      return func(args...);  
   }
   static std::function<Ret(Params...)> func; 
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

namespace mr
{

static void OnWindowClose(GLFWwindow *window)
{
    // Window *win = WindowManager::GetInstance().GetWindowByHandle(window);

    // WindowCloseEvent windowCloseEvent = {};
    // windowCloseEvent.window = win;

    // InputEvent event = {};
    // event.type = InputEventType::WINDOW_CLOSE;
    // event.event.windowClose = windowCloseEvent;

    // win->input.SubmitEvent(event);
    Application::GetInstance().Close();
}

static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    printf("key %d\n", key);
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        Application::GetInstance().Close();
    }
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
    // glfwSetWindowCloseCallback(this->window, OnWindowClose);
    Callback<void(GLFWwindow*)>::func = std::bind(&GlfwWindow::OnClose, this, std::placeholders::_1);
    GLFWwindowclosefun onCloseFn = static_cast<GLFWwindowclosefun>(Callback<void(GLFWwindow*)>::callback);
    glfwSetWindowCloseCallback(this->window, (GLFWwindowclosefun)onCloseFn);
    glfwSetKeyCallback(this->window, OnKeyEvent);
}

void GlfwWindow::Close()
{

}

void GlfwWindow::OnClose(GLFWwindow *window)
{
    Application::GetInstance().Close();
}

GlfwWindow::~GlfwWindow()
{
    WindowManager &windowManager = WindowManager::GetInstance();
    
    windowManager.RemoveWindow(this);
    if(this->window)
    {
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

window_handle GlfwWindow::GetHandle() { return (window_handle)this->window; }

} // namespace mr
