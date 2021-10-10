#include <mobrend/window.h>
#include <mobrend/glfw/window.h>

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
