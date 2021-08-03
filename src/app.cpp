#include <iostream>
#include "mr_application.h"

class UserApp : public mr::Program
{
public:
    UserApp(){}
    ~UserApp(){}

    virtual void OnStart() override
    {
    }

    virtual void OnUpdate() override
    {
    }

    virtual void OnRender(mr::Renderer *renderer) override
    {
    }

    virtual void OnGuiRender() override
    {
    }

    virtual void OnDestroy() override
    {
    }

private:
};

int main(int argc, char *argv[])
{
    mr::Application::RunParams params = {};
    params.program = new UserApp();

    params.windowCreateParams.width = 1280;
    params.windowCreateParams.height = 720;
    params.windowCreateParams.windowName = "MobRend";

    mr::Application::GetInstance().Run(params);
    
    delete(params.program);
    return 0;
}