#include <iostream>
#include <unordered_map>
#include "mr_application.h"
#include "mr_logger.h"

void OnUpdate();

int main(int argc, char *argv[])
{
    mr::Application::RunParams params = {};
    params.onUpdate = OnUpdate;

    params.windowCreateParams.width = 1280;
    params.windowCreateParams.height = 720;
    params.windowCreateParams.windowName = "MobRend";

    mr::Application::GetInstance().Run(params);
    return 0;
}

void OnUpdate()
{
    mrlog("hey");
}