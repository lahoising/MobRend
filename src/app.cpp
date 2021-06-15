#include <iostream>
#include <unordered_map>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "mr_application.h"
#include "mr_logger.h"
#include "mr_model.h"
#include "mr_shader.h"
#include "mr_fps_camera.h"
#include "mr_light.h"

class UserApp : public mr::Program
{
public:
    UserApp(){}
    ~UserApp(){}

    virtual void OnStart() override
    {
        mr::DirectionalLight *dirLight = new mr::DirectionalLight(
            glm::vec3(1.0f, 1.0f, 1.0f), 1.0f
        );
        dirLight->direction = glm::normalize(glm::vec3(0.0f, -0.2f, 0.8f));
        this->directional = dirLight;

        mr::Shader::CreateParams shaderCreateParams = {};
        shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\default_shader.vert.spv";
        shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\default_shader.frag.spv";
        shader = mr::Shader::Create(shaderCreateParams);

        cam = mr::FPSCamera();
        mr::Camera::Config camConfig = {};
        camConfig.perspective.fov = glm::radians(60.0f);
        camConfig.perspective.aspectRatio = 1280.0f / 720.0f;
        camConfig.perspective.near = 0.1f;
        camConfig.perspective.far = 1000.0f;
        cam.camera = mr::Camera(
            mr::Camera::Type::PERSPECTIVE,
            camConfig
        );
        cam.movementSpeed = 0.03f;

        tex = mr::Texture::Create("D:\\Pictures\\Screenshots\\Screenshot (44).png");
        specMap = mr::Texture::Create("D:\\Documents\\progs\\krita_resources\\MobRend\\IU_Spec.png");

        model = mr::Model::Load("D:\\Documents\\git\\MobRend\\resources\\models\\kunai.fbx");

        this->isCursonVisible = false;

        mr::Application &app = mr::Application::GetInstance();
        app.GetMainWindow()->SetCursorVisible(isCursonVisible);
    }

    virtual void OnUpdate() override
    {
        mr::Window *mainWindow = mr::Application::GetInstance().GetMainWindow();
        mr::Input &input = mainWindow->input;
        if(input.KeyJustPressed(81))
        {
            mainWindow->SetCursorVisible(this->isCursonVisible = !this->isCursonVisible);
        }

        if(input.KeyJustPressed(256))
        {
            mr::Application::GetInstance().Close();
        }
    }

    virtual void OnRender(mr::Renderer *renderer) override
    {
        glm::mat4 identityMat = glm::identity<glm::mat4>();
        cam.Update();

        shader->Bind();

        shader->UploadMat4(
            "u_cam.viewProjection",
            cam.camera.GetViewProjection()
        );

        shader->UploadMat4(
            "u_cam.model",
            identityMat
        );

        glm::mat3 normalMat = glm::transpose(glm::inverse(identityMat));
        shader->UploadMat3(
            "u_cam.normalMat",
            normalMat
        );

        shader->UploadVec3(
            "u_scene.color",
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        directional->Bind(shader, "u_scene.ambientLight");
        
        shader->UploadVec3(
            "u_scene.viewPos",
            cam.camera.GetPosition()
        );

        shader->UploadVec3("u_scene.phongMaterial.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader->UploadFloat("u_scene.phongMaterial.diffuseMapStrength", 0.0f);
        
        shader->UploadVec3("u_scene.phongMaterial.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader->UploadFloat("u_scene.phongMaterial.specularMapStrength", 0.0f);
        shader->UploadFloat("u_scene.phongMaterial.shininess", 32.f);

        shader->UploadTexture2D("u_diffuseMap", tex);
        shader->UploadTexture2D("u_specularMap", specMap);

        mr::Renderer::Command cmd = {};
        cmd.topologyType = mr::TOPOLOGY_TRIANGLES;
        cmd.model = model;
        cmd.renderObjectType = mr::RENDER_OBJECT_MODEL;

        renderer->Render(cmd);
    }

    virtual void OnGuiRender() override
    {
        ImGui::Begin("Scene Settings");
        {
            ImGui::DragFloat("Camera Movement Speed", &this->cam.movementSpeed, 0.001f);
            ImGui::DragFloat("Camera Sensitivity", &this->cam.sensitivity, 0.01f);

            if(ImGui::DragFloat3("Light Direction", glm::value_ptr(this->directional->direction), 0.01f))
            {
                this->directional->direction = glm::normalize(this->directional->direction);
            }
        }
        ImGui::End();
    }

    virtual void OnDestroy() override
    {
        delete(this->model);
        delete(this->directional);
        delete(this->shader);
    }

private:
    mr::Shader *shader = nullptr;

    mr::FPSCamera cam;
    mr::Texture *tex = nullptr;
    mr::Texture *specMap = nullptr;

    mr::DirectionalLight *directional = nullptr;

    mr::Model *model = nullptr;
    bool isCursonVisible;
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