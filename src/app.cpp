#include <iostream>
#include <unordered_map>
#include <chrono>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "mr_asset_manager.h"
#include "mr_application.h"
#include "mr_logger.h"
#include "mr_model.h"
#include "mr_shader.h"
#include "mr_fps_camera.h"
#include "mr_light.h"
#include "mr_framebuffer.h"
#include "mr_uniform_buffer.h"
#include "mr_input.h"

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