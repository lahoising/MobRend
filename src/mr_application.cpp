#include <iostream>
#include "mr_application.h"
#include "mr_window.h"

namespace mr
{
    
Application::Application(){}

Application::~Application()
{
    printf("app bye!\n");
    delete(this->window);
}

void Application::Run(Application::RunParams params)
{
    printf("app init\n");

    Application::InitParams initParams = {};
    initParams.windowCreateParams = params.windowCreateParams;

    this->running = this->Init(initParams);
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
    return true;
}

void Application::Update()
{
    this->window->SwapBuffers();
    if(this->window->input.WindowShouldClose() || this->window->input.KeyJustReleased(256))
        this->Close();
}

void Application::Close()
{
    this->running = false;
}


} // namespace mr