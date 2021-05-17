#include <iostream>
#include <unordered_map>
#include "mr_application.h"

void OnUpdate();

int main(int argc, char *argv[])
{
    mr::Application::RunParams params = {};
    params.onUpdate = OnUpdate;

    mr::Application::GetInstance().Run(params);
    printf("no se destruye?\n");

    return 0;
}

void OnUpdate()
{
    printf("update\n");
}