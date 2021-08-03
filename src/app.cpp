#include <iostream>
#include "mr_application.h"
#include "mr_input.h"
#include "mr_logger.h"

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
        mr::Input &input = mr::Application::GetInstance().GetMainWindow()->input;

        if(input.KeyJustPressed(mr::KEY_W))
        {
            mrwarn("pressed w!");
        }

        glm::vec2 delta = input.GetMouseDelta();
        if( delta.x + delta.y != 0.0f )
        {
            mrlog("mouse delta: %.2f, %.2f", delta.x, delta.y);
        }
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