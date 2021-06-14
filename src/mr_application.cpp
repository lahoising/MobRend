#include <iostream>
#include "mr_application.h"
#include "mr_window.h"
#include "mr_gui.h"

#include "mr_image_loader.h"
#include "mr_logger.h"

namespace mr
{
    
Application::Application(){}

Application::~Application()
{
    delete(this->gui);
    delete(this->renderer);
    delete(this->window);
    printf("app bye!\n");
}

void Application::Run(Application::RunParams params)
{
    printf("app init\n");

    Application::InitParams initParams = {};
    initParams.windowCreateParams = params.windowCreateParams;

    this->running = this->Init(initParams);
    if(!this->running) return;

    params.program->OnStart();

    while(this->running)
    {
        this->Update();
        params.program->OnUpdate();

        this->Render(params.program);
    }

    params.program->OnDestroy();
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
    this->window->PollEvents();
    if(this->window->input.WindowShouldClose())
        this->Close();
}

void Application::Render(Program *prog)
{
    this->renderer->OnRenderBegin();
    prog->OnRender(this->renderer);

    this->gui->BeginFrame();
    prog->OnGuiRender();
    this->gui->EndFrame();

    this->renderer->OnRenderEnd();

    this->window->SwapBuffers();
}

void Application::Close()
{
    this->running = false;
}


} // namespace mr