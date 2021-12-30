#include <imgui.h>

#include <mobrend/gui.h>
#include <mobrend/application.h>

#ifdef MOBREND_GLFW_WINDOW
#include <imgui_impl_glfw.h>
#endif

#ifdef MOBREND_GL_RENDERING
#include <imgui_impl_opengl3.h>
#endif

namespace mr
{
    
Gui::Gui(Gui::CreateParams params)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    #ifdef MOBREND_GLFW_WINDOW
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)params.windowHandle, true);
    #endif

    #ifdef MOBREND_GL_RENDERING
    ImGui_ImplOpenGL3_Init((char*)params.rendererInitInfo);
    #endif
}

Gui::~Gui()
{
    #ifdef MOBREND_GL_RENDERING
    ImGui_ImplOpenGL3_Shutdown();
    #endif
    
    #ifdef MOBREND_GLFW_WINDOW
    ImGui_ImplGlfw_Shutdown();
    #endif
    
    ImGui::DestroyContext();
}

void Gui::BeginFrame()
{
    #ifdef MOBREND_GL_RENDERING
    ImGui_ImplOpenGL3_NewFrame();
    #endif
    
    #ifdef MOBREND_GLFW_WINDOW
    ImGui_ImplGlfw_NewFrame();
    #endif

    ImGui::NewFrame();
}

void Gui::EndFrame()
{
    ImGui::Render();

    #ifdef MOBREND_GL_RENDERING
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #endif
}


} // namespace mr
