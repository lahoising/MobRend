#ifdef MOBREND_GL_RENDERING

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#ifdef MOBREND_GLFW_WINDOW
#include "mr_platform/mr_glfw_window.h"
#include <imgui_impl_glfw.h>
#endif

#include "mr_platform/mr_gl_renderer.h"
#include "mr_application.h"

#ifdef MOBREND_GLFW_WINDOW
#include <GLFW/glfw3.h>
#endif

namespace mr
{
    
GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glClearColor(0.8f, 0.3f, 0.2f, 1.0f);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    GlfwWindow *win = (GlfwWindow*)Application::GetInstance().GetMainWindow();
    GLFWwindow *window = win->GetWindow();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

GlRenderer::~GlRenderer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GlRenderer::SetViewport(int x, int y, int width, int height) 
{
    glViewport(x, y, width, height);
}

void GlRenderer::OnRenderBegin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
}

void GlRenderer::OnRenderEnd()
{

}


} // namespace mr


#endif