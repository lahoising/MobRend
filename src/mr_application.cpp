#include <iostream>
#include "mr_application.h"
#include "mr_window.h"
#include "mr_gui.h"

namespace mr
{
    
Application::Application(){}

Application::~Application()
{
    printf("app bye!\n");
    delete(this->gui);
    delete(this->renderer);
    delete(this->window);
}

void Application::Run(Application::RunParams params)
{
    printf("app init\n");

    Application::InitParams initParams = {};
    initParams.windowCreateParams = params.windowCreateParams;

    this->running = this->Init(initParams);
    if(!this->running) return;

    if(params.onStart)
        params.onStart();

    while(this->running)
    {
        this->Update();
        if(params.onUpdate)
            params.onUpdate();
    }

    printf("sal de aqui\n");
    if(params.onDestroy)
        params.onDestroy();
}

bool Application::Init(Application::InitParams params)
{
    this->window = Window::Create(params.windowCreateParams);
    if(!this->window) return false;

    this->renderer = Renderer::Create();
    if(!this->renderer) return false;

    Gui::CreateParams guiInitParams = {};
    guiInitParams.windowHandle = this->window->GetHandle();
    guiInitParams.rendererInitInfo = this->renderer->GetGuiInitInfo();
    this->gui = new Gui(guiInitParams);
    this->renderer->DeleteGuiInitInfo(guiInitParams.rendererInitInfo);
    if(!this->gui) return false;

    return true;
}

void Application::Update()
{
    this->window->SwapBuffers();
    if(this->window->input.WindowShouldClose() || this->window->input.KeyJustReleased(256))
        this->Close();

    this->renderer->OnRenderBegin();

    this->gui->BeginFrame();
    this->gui->EndFrame();

    this->renderer->OnRenderEnd();
}

void Application::Close()
{
    this->running = false;
}


} // namespace mr