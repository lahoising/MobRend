#ifdef MOBREND_GLFW_WINDOW
#include <unordered_map>

#include "mr_platform/mr_glfw_window.h"
#include "mr_application.h"

#ifdef MOBREND_GL_RENDERING
#include "glad/glad.h"
#endif

namespace mr
{

static std::unordered_map<GLFWwindow*,GlfwWindow*> windowMap;

static void OnGlfwWindowClose(GLFWwindow *win)
{
    GlfwWindow *window = windowMap[win];
    
    WindowCloseEvent windowCloseEvent = {};
    windowCloseEvent.window = (Window*)window;

    InputEvent event = {};
    event.type = InputEventType::WINDOW_CLOSE;
    event.windowClose = windowCloseEvent;

    window->input.SubmitEvent(event);
}

static void OnGlfwKeyPressed(GLFWwindow *win, int key, int scancode, int action, int mod)
{
    GlfwWindow *window = windowMap[win];

    KeyEvent keyEvent = {};
    keyEvent.key = key;
    keyEvent.mod = mod;
    keyEvent.pressed = action;

    InputEvent event = {};
    event.type = InputEventType::KEY_EVENT;
    event.keyEvent = keyEvent;

    window->input.SubmitEvent(event);
}

static void OnGlfwFramebufferSizeChange(GLFWwindow *win, int w, int h)
{
    glfwMakeContextCurrent(win);
    Application::GetInstance().GetRenderer()->SetViewport(0, 0, w, h);
}

static void OnGlfwError(int errorCode, const char *description)
{
    printf("Window Error %d: %s\n", errorCode, description);
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

        glfwSetErrorCallback(OnGlfwError);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef MOBREND_GL_RENDERING
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

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
    glfwSetKeyCallback(this->window, OnGlfwKeyPressed);
    glfwSetFramebufferSizeCallback(this->window, OnGlfwFramebufferSizeChange);
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
#endif