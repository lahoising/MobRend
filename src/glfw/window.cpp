#ifdef MOBREND_GLFW_WINDOW
#include <unordered_map>

#include "mr_logger.h"
#include <mobrend/glfw/window.h>
#include <mobrend/application.h>

#ifdef MOBREND_GL_RENDERING
#include "glad/glad.h"
#endif

namespace mr
{

static std::unordered_map<GLFWwindow*,GlfwWindow*> windowMap;

static KeyCode GetKeyCode(int glfwKey)
{
    switch (glfwKey)
    {
    case GLFW_KEY_Q: return KEY_Q;
    case GLFW_KEY_W: return KEY_W;
    case GLFW_KEY_E: return KEY_E;
    case GLFW_KEY_R: return KEY_R;
    case GLFW_KEY_T: return KEY_T;
    case GLFW_KEY_Y: return KEY_Y;
    case GLFW_KEY_U: return KEY_U;
    case GLFW_KEY_I: return KEY_I;
    case GLFW_KEY_O: return KEY_O;
    case GLFW_KEY_P: return KEY_P;
    case GLFW_KEY_A: return KEY_A;
    case GLFW_KEY_S: return KEY_S;
    case GLFW_KEY_D: return KEY_D;
    case GLFW_KEY_F: return KEY_F;
    case GLFW_KEY_G: return KEY_G;
    case GLFW_KEY_H: return KEY_H;
    case GLFW_KEY_J: return KEY_J;
    case GLFW_KEY_K: return KEY_K;
    case GLFW_KEY_L: return KEY_L;
    case GLFW_KEY_Z: return KEY_Z;
    case GLFW_KEY_X: return KEY_X;
    case GLFW_KEY_C: return KEY_C;
    case GLFW_KEY_V: return KEY_V;
    case GLFW_KEY_B: return KEY_B;
    case GLFW_KEY_N: return KEY_N;
    case GLFW_KEY_M: return KEY_M;
    case GLFW_KEY_COMMA: return KEY_COMMA;
    case GLFW_KEY_PERIOD: return KEY_PERIOD;
    case GLFW_KEY_SLASH: return KEY_SLASH;
    case GLFW_KEY_MINUS: return KEY_DASH;
    case GLFW_KEY_BACKSLASH: return KEY_BACKSLASH;
    case GLFW_KEY_EQUAL: return KEY_EQUALS;
    case GLFW_KEY_LEFT_BRACKET: return KEY_LBRACKET;
    case GLFW_KEY_RIGHT_BRACKET: return KEY_RBRACKET;
    case GLFW_KEY_ESCAPE: return KEY_ESC;
    case GLFW_KEY_APOSTROPHE: return KEY_APOSTROPHE;
    case GLFW_KEY_1: return KEY_1;
    case GLFW_KEY_2: return KEY_2;
    case GLFW_KEY_3: return KEY_3;
    case GLFW_KEY_4: return KEY_4;
    case GLFW_KEY_5: return KEY_5;
    case GLFW_KEY_6: return KEY_6;
    case GLFW_KEY_7: return KEY_7;
    case GLFW_KEY_8: return KEY_8;
    case GLFW_KEY_9: return KEY_9;
    case GLFW_KEY_0: return KEY_0;
    case GLFW_KEY_SEMICOLON: return KEY_SEMICOLON;
    case GLFW_KEY_GRAVE_ACCENT: return KEY_GRAVE;
    case GLFW_KEY_LEFT: return KEY_LEFT;
    case GLFW_KEY_RIGHT: return KEY_RIGHT;
    case GLFW_KEY_UP: return KEY_UP;
    case GLFW_KEY_DOWN: return KEY_DOWN;
    case GLFW_KEY_SPACE: return KEY_SPACE;
    case GLFW_KEY_BACKSPACE: return KEY_BACKSPACE;
    case GLFW_KEY_HOME: return KEY_HOME;
    case GLFW_KEY_END: return KEY_END;
    case GLFW_KEY_PAGE_UP: return KEY_PAGE_UP;
    case GLFW_KEY_PAGE_DOWN: return KEY_PAGE_DOWN;
    case GLFW_KEY_CAPS_LOCK: return KEY_CAPS_LOCK;
    case GLFW_KEY_PRINT_SCREEN: return KEY_PRINT_SCREEN;
    case GLFW_KEY_DELETE: return KEY_DELETE;
    case GLFW_KEY_INSERT: return KEY_INSERT;
    case GLFW_KEY_NUM_LOCK: return KEY_NUM_LOCK;
    case GLFW_KEY_F1: return KEY_F1;
    case GLFW_KEY_F2: return KEY_F2;
    case GLFW_KEY_F3: return KEY_F3;
    case GLFW_KEY_F4: return KEY_F4;
    case GLFW_KEY_F5: return KEY_F5;
    case GLFW_KEY_F6: return KEY_F6;
    case GLFW_KEY_F7: return KEY_F7;
    case GLFW_KEY_F8: return KEY_F8;
    case GLFW_KEY_F9: return KEY_F9;
    case GLFW_KEY_F10: return KEY_F10;
    case GLFW_KEY_F11: return KEY_F11;
    case GLFW_KEY_F12: return KEY_F12;
    case GLFW_KEY_F13: return KEY_F13;
    case GLFW_KEY_F14: return KEY_F14;
    case GLFW_KEY_F15: return KEY_F15;
    case GLFW_KEY_F16: return KEY_F16;
    case GLFW_KEY_F17: return KEY_F17;
    case GLFW_KEY_F18: return KEY_F18;
    case GLFW_KEY_F19: return KEY_F19;
    case GLFW_KEY_F20: return KEY_F20;
    case GLFW_KEY_F21: return KEY_F21;
    case GLFW_KEY_F22: return KEY_F22;
    case GLFW_KEY_F23: return KEY_F23;
    case GLFW_KEY_F24: return KEY_F24;
    case GLFW_KEY_F25: return KEY_F25;
    case GLFW_KEY_TAB: return KEY_TAB;
    case GLFW_KEY_LEFT_CONTROL: return KEY_LCTRL;
    case GLFW_KEY_LEFT_SHIFT: return KEY_LSHIFT;
    case GLFW_KEY_LEFT_ALT: return KEY_LALT;
    case GLFW_KEY_LEFT_SUPER: return KEY_LSUPER;
    case GLFW_KEY_RIGHT_CONTROL: return KEY_RCTRL;
    case GLFW_KEY_RIGHT_SHIFT: return KEY_RSHIFT;
    case GLFW_KEY_RIGHT_ALT: return KEY_RALT;
    case GLFW_KEY_RIGHT_SUPER: return KEY_RSUPER;
    default: return KeyCode::KEY_UNKNOWN;
    }
}

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
    keyEvent.key = GetKeyCode(key);
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

static void OnGlfwMousePosition(GLFWwindow *win, double x, double y)
{
    GlfwWindow *window = windowMap[win];
    
    MousePositionEvent mouseEvent = {};
    mouseEvent.x = (float)x;
    mouseEvent.y = (float)y;

    InputEvent event = {};
    event.type = InputEventType::MOUSE_POSITION;
    event.mousePosition = mouseEvent;

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
    glfwSetCursorPosCallback(this->window, OnGlfwMousePosition);
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
        mrlog("window bye!\n");
    }
}

void GlfwWindow::PollEvents()
{
    this->input.SwapState();
    glfwPollEvents();
}

void GlfwWindow::SwapBuffers()
{
    glfwSwapBuffers(this->window);
}

void GlfwWindow::SetCursorVisible(bool visible)
{
    glfwSetInputMode(this->window, GLFW_CURSOR, visible? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

uint32_t GlfwWindow::GetWidth()  
{
    int width = 0;
    glfwGetWindowSize(this->window, &width, nullptr);
    return (uint32_t)width;
}

uint32_t GlfwWindow::GetHeight() 
{
    int height = 0;
    glfwGetWindowSize(this->window, nullptr, &height);
    return (uint32_t)height;
}

uint32_t GlfwWindow::GetFramebufferWidth()
{
    int width = 0;
    glfwGetFramebufferSize(this->window, &width, nullptr);
    return (uint32_t)width;
}

uint32_t GlfwWindow::GetFramebufferHeight()
{
    int height = 0;
    glfwGetFramebufferSize(this->window, nullptr, &height);
    return (uint32_t)height;
}


} // namespace mr
#endif
