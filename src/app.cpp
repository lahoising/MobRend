#include <iostream>
#include "mr_application.h"

void OnUpdate();

int main(int argc, char *argv[])
{
    mr::Application::RunParams params = {};
    params.onUpdate = OnUpdate;

    mr::Application::GetInstance().Run(params);

    return 0;
}

void OnUpdate()
{
    printf("update\n");
}