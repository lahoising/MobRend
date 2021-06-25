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
#include "mr_framebuffer.h"
#include "mr_uniform_buffer.h"

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
        shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\simple.vert.spv";
        shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\simple.frag.spv";
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

        mr::Texture::LoadParams textureLoadParams = {};
        textureLoadParams.type = mr::Texture::TEXTURE_TYPE_2D;
        textureLoadParams.filepath = "D:\\Documents\\Art\\Sprites\\Exports\\alphas.png";
        tex = mr::Texture::Load(textureLoadParams);
        
        mr::VertexLayout layout = {
            { mr::ATTRIBUTE_TYPE_FLOAT, 3 },
            { mr::ATTRIBUTE_TYPE_FLOAT, 2 }
        };

        float cubeVertices[] = {
             1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,    1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f,    0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,

        //      1.0f, -1.0f,  1.0f,    1.0f, 0.0f,
        //      1.0f,  1.0f,  1.0f,    1.0f, 1.0f,
        //      1.0f,  1.0f, -1.0f,    0.0f, 1.0f,
        //      1.0f, -1.0f, -1.0f,    0.0f, 0.0f,

        //     -1.0f, -1.0f,  1.0f,    1.0f, 0.0f,
        //     -1.0f,  1.0f,  1.0f,    1.0f, 1.0f,
        //      1.0f,  1.0f,  1.0f,    0.0f, 1.0f,
        //      1.0f, -1.0f,  1.0f,    0.0f, 0.0f,
        };

        uint32_t cubeIndices[] = {
            0, 1, 2, 2, 3, 0,
        };

        mr::Mesh::CreateParams meshCreateParams = {};
        meshCreateParams.vertexLayout = &layout;
        meshCreateParams.vertices = cubeVertices;
        meshCreateParams.verticesArraySize = sizeof(cubeVertices);
        meshCreateParams.indices = cubeIndices;
        meshCreateParams.indexCount = sizeof(cubeIndices) / sizeof(uint32_t);
        this->cube = new mr::Mesh(meshCreateParams);
        // model = mr::Model::Load("D:\\Documents\\git\\MobRend\\resources\\models\\kunai.fbx");

        this->isCursonVisible = false;

        mr::Application &app = mr::Application::GetInstance();
        mr::Window *window = app.GetMainWindow();
        window->SetCursorVisible(isCursonVisible);

        mr::Renderer *rend = app.GetRenderer();
        rend->EnableRenderPass(
            mr::RENDER_PASS_CULLING, false
        );

        mr::UniformBuffer::CreateParams uboCreateParams = {};
        uboCreateParams.binding = 0;
        uboCreateParams.bufferSize = sizeof(glm::mat4);
        this->camUBO = mr::UniformBuffer::Create(uboCreateParams);
        this->shader->UploadUniformBuffer("CamMatrices", this->camUBO);
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
        mr::Renderer::Command cmd = {};

        glm::mat4 identityMat = glm::identity<glm::mat4>();
        cam.Update();
        
        this->camUBO->SetData(glm::value_ptr(cam.camera.GetViewProjection()), sizeof(glm::mat4), 0);

        this->shader->Bind();
        this->shader->UploadMat4("u_model.model", identityMat);
        this->shader->UploadTexture("u_texture", this->tex);

        cmd = {};
        cmd.mesh = this->cube;
        cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
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
        delete(this->camUBO);
        delete(this->model);
        delete(this->cube);
        delete(this->directional);
        delete(this->shader);
    }

private:
    mr::Shader *shader = nullptr;
    mr::UniformBuffer *camUBO = nullptr;

    mr::FPSCamera cam;
    mr::Texture *tex = nullptr;

    mr::DirectionalLight *directional = nullptr;

    mr::Model *model = nullptr;
    mr::Mesh *cube = nullptr;
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