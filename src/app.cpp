#include <iostream>
#include <glm/glm.hpp>
#include <imgui.h>
#include "mr_application.h"
#include "mr_mesh.h"
#include "mr_model.h"
#include "mr_shader.h"
#include "mr_fps_camera.h"

class UserApp : public mr::Program
{
public:
    UserApp(){}
    ~UserApp(){}

    virtual void OnStart() override
    {
        mr::Shader::CreateParams shaderParams = {};
        mr::AssetManager::GetAssetPath(shaderParams.vertFilePath, "resources/shaders/solid_color.vert.spv");
        mr::AssetManager::GetAssetPath(shaderParams.fragFilePath, "resources/shaders/solid_color.frag.spv");
        this->shader = mr::Shader::Create(shaderParams);
        
        char filepath[265] = {};
        mr::AssetManager::GetAssetPath(filepath, "resources/models/kunai.fbx");
        this->model = mr::Model::Load(filepath);

        this->cam = mr::FPSCamera();
        mr::Camera::Config camConfig = {};
        camConfig.perspective.aspectRatio = 16.0f/9.0f;
        camConfig.perspective.far = 1000.0f;
        camConfig.perspective.fov = 90.0f;
        camConfig.perspective.near = 0.1f;
        this->cam.camera.SetConfiguration(mr::Camera::PERSPECTIVE, camConfig);

        this->cam.movementSpeed = 0.01f;
    }

    virtual void OnUpdate() override
    {
        this->cam.Update();

        mr::Window *mainWindow = mr::Application::GetInstance().GetMainWindow();
        mr::Input &input = mainWindow->input;
        if(input.KeyJustPressed(mr::KEY_ESC))
        {
            mr::Application::GetInstance().Close();
        }

        if(input.KeyJustPressed(mr::KeyCode::KEY_Q))
        {
            mainWindow->SetCursorVisible(this->isCursorVisible = !this->isCursorVisible);
        }
    }

    virtual void OnRender(mr::Renderer *renderer) override
    {
        this->shader->Bind();
        this->shader->UploadMat4("u_cam.viewProjection", this->cam.camera.GetViewProjection());
        this->shader->UploadMat4("u_cam.model", glm::identity<glm::mat4>());

        mr::Renderer::Command cmd = {};
        cmd.model = this->model;
        cmd.renderObjectType = mr::RENDER_OBJECT_MODEL;
        renderer->Render(cmd);
    }

    virtual void OnGuiRender() override
    {
        // ImGui::Begin("Scene Settings");
        {
            // ImGui::DragFloat("Camera Movement Speed", &this->cam.movementSpeed, 0.001f);
            // ImGui::DragFloat("Camera Sensitivity", &this->cam.sensitivity, 0.01f);
        }
        // ImGui::End();
    }

    virtual void OnDestroy() override
    {
        delete(this->model);
        delete(this->shader);
    }

private:
    mr::Model *model = nullptr;
    mr::Shader *shader = nullptr;
    mr::FPSCamera cam;
    bool isCursorVisible = false;
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