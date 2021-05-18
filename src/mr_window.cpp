#include "mr_window.h"
#include "mr_platform/mr_glfw_window.h"

namespace mr
{
    
Window *Window::Create(Window::CreateParams params)
{
    #ifdef MOBREND_GLFW_WINDOW
    return new GlfwWindow(params);
    #else
    return nullptr;
    #endif
}

} // namespace mr
