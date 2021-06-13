#include <iostream>
#include <unordered_map>
#include "mr_application.h"
#include "mr_logger.h"
#include "mr_model.h"

class UserApp
{
public:
    UserApp()
    {
        // this->model = mr::Model::Load();
    }

    ~UserApp()
    {
        // delete(this->model);
    }

    void Render(mr::Renderer *renderer)
    {

    }

private:
    mr::Model *model;
};
UserApp myApp;

void OnUpdate();
void OnRender(mr::Renderer *renderer);

int main(int argc, char *argv[])
{
    mr::Application::RunParams params = {};
    params.onUpdate = OnUpdate;
    params.onRender = OnRender;

    params.windowCreateParams.width = 1280;
    params.windowCreateParams.height = 720;
    params.windowCreateParams.windowName = "MobRend";

    myApp = UserApp();
    mr::Application::GetInstance().Run(params);
    return 0;
}

void OnUpdate()
{
    mrlog("hey");
}

void OnRender(mr::Renderer *renderer)
{
    myApp.Render(renderer);
}