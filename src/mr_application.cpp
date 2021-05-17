#include <iostream>
#include "mr_application.h"
#include "mr_platform/mr_glfw_window.h"

namespace mr
{
    
Application::Application(){}

Application::~Application()
{
    printf("app bye!\n");
    delete(this->window);
}

void Application::Run(RunParams params)
{
    printf("app init\n");

    this->running = this->Init();
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

bool Application::Init()
{
    Window::CreateParams windowCreateParams = {};
    windowCreateParams.width = 800;
    windowCreateParams.height = 600;
    windowCreateParams.windowName = "ModRend";
    this->window = new GlfwWindow(windowCreateParams);
    return true;
}

void Application::Update()
{
    this->window->SwapBuffers();
    if(this->window->input.WindowShouldClose())
        this->Close();
}

void Application::Close()
{
    this->running = false;
}


} // namespace mr