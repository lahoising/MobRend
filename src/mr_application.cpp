#include <iostream>
#include "mr_application.h"

namespace mr
{
    
Application::Application(){}

Application::~Application()
{
    printf("app bye!\n");
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

    if(params.onDestroy)
        params.onDestroy();
}

bool Application::Init()
{
    return true;
}

void Application::Update()
{
    this->Close();
}

void Application::Close()
{
    this->running = false;
}


} // namespace mr