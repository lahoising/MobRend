#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "mr_application.h"
#include "mr_mesh.h"
#include "mr_model.h"
#include "mr_shader.h"
#include "mr_fps_camera.h"
#include "mr_light.h"
#include "mr_uniform_buffer.h"

class UserApp : public mr::Program
{
public:
    UserApp(){}
    ~UserApp(){}

    virtual void OnStart() override
    {
        mr::Shader::CreateParams shaderParams = {};
        mr::AssetManager::GetAssetPath(shaderParams.vertFilePath, "resources/shaders/default_shader.vert.spv");
        mr::AssetManager::GetAssetPath(shaderParams.fragFilePath, "resources/shaders/default.frag.spv");
        this->shader = mr::Shader::Create(shaderParams);
        
        this->model = mr::Model::Load("D:\\Documents\\Clases\\4th Year\\sem-2\\it386\\sprints\\3rd\\Meshes\\SM_Roof.fbx");

        this->cam = mr::FPSCamera();
        mr::Camera::Config camConfig = {};
        camConfig.perspective.aspectRatio = 16.0f/9.0f;
        camConfig.perspective.far = 1000.0f;
        camConfig.perspective.fov = glm::radians(60.0f);
        camConfig.perspective.near = 0.1f;
        this->cam.camera.SetConfiguration(mr::Camera::PERSPECTIVE, camConfig);

        // this->cam.movementSpeed = 0.01f;

        mr::DirectionalLight *dirLight = new mr::DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
        dirLight->direction = glm::vec3(0.0f, -1.0f, 1.0f);
        this->light = dirLight;

        mr::UniformBuffer::CreateParams uboCreateParams = {};
        uboCreateParams.binding = 0;
        uboCreateParams.bufferSize = sizeof(glm::mat4) * 2;
        this->camUBO = mr::UniformBuffer::Create(uboCreateParams);
        this->shader->UploadUniformBuffer("CameraMatrices", this->camUBO);
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
        glm::mat4 identityMat = glm::identity<glm::mat4>();

        this->shader->Bind();
        this->light->Bind(this->shader, "u_scene.directional");
        this->shader->UploadVec4("u_scene.material.diffuse", {1.0f, 0.5f, 0.2f, 1.0f});
        this->shader->UploadFloat("u_scene.material.diffuseMapStrength", 0.0f);
        this->shader->UploadVec4("u_scene.material.specular", {0.1f, 0.2f, 1.0f, 1.0f});
        this->shader->UploadFloat("u_scene.material.specularMapStrength", 0.0f);
        this->shader->UploadFloat("u_scene.material.shininess", 12.0f);
        this->shader->UploadVec3("u_scene.viewPos", this->cam.camera.GetPosition());

        glm::mat4 normalMat = glm::mat3(glm::transpose(glm::inverse(identityMat)));
        float matrices[16 + 16] = {};
        memcpy(matrices, glm::value_ptr(cam.camera.GetViewProjection()), sizeof(glm::mat4));
        memcpy(matrices + 16, glm::value_ptr(normalMat), sizeof(glm::mat4));
        this->camUBO->SetData(matrices, sizeof(glm::mat4) * 2, 0);
        this->shader->UploadMat4("u_model.model", identityMat);

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
        delete(this->light);
        delete(this->camUBO);
    }

private:
    mr::Model *model = nullptr;
    mr::Shader *shader = nullptr;
    mr::Light *light = nullptr;
    mr::FPSCamera cam;
    mr::UniformBuffer *camUBO = nullptr;
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