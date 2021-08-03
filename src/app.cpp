#include <iostream>
#include <glm/glm.hpp>
#include "mr_application.h"
#include "mr_mesh.h"
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
        
        mr::VertexLayout layout = 
        {
            {mr::ATTRIBUTE_TYPE_FLOAT, 3},
            {mr::ATTRIBUTE_TYPE_FLOAT, 3},
            {mr::ATTRIBUTE_TYPE_FLOAT, 2}
        };

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,      0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, -1.0f,  0.0f, 0.0f
        };

        uint32_t indices[] = {
            0, 1, 2
        };

        mr::Mesh::CreateParams meshParams = {};
        meshParams.vertexLayout = &layout;
        meshParams.vertices = vertices;
        meshParams.verticesArraySize = sizeof(vertices);
        meshParams.indexCount = sizeof(indices) / sizeof(uint32_t);
        meshParams.indices = indices;

        this->mesh = new mr::Mesh(meshParams);

        this->cam = mr::FPSCamera();
        mr::Camera::Config camConfig = {};
        camConfig.perspective.aspectRatio = 16.0f/9.0f;
        camConfig.perspective.far = 1000.0f;
        camConfig.perspective.fov = 90.0f;
        camConfig.perspective.near = 0.1f;
        this->cam.camera.SetConfiguration(mr::Camera::PERSPECTIVE, camConfig);
    }

    virtual void OnUpdate() override
    {
        this->cam.Update();

        mr::Input &input = mr::Application::GetInstance().GetMainWindow()->input;
        if(input.KeyJustPressed(mr::KEY_ESC))
        {
            mr::Application::GetInstance().Close();
        }
    }

    virtual void OnRender(mr::Renderer *renderer) override
    {
        this->shader->Bind();
        this->shader->UploadMat4("u_cam.viewProjection", this->cam.camera.GetViewProjection());
        this->shader->UploadMat4("u_cam.model", glm::identity<glm::mat4>());

        mr::Renderer::Command cmd = {};
        cmd.mesh = this->mesh;
        cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
        renderer->Render(cmd);
    }

    virtual void OnGuiRender() override
    {
    }

    virtual void OnDestroy() override
    {
        delete(this->mesh);
        delete(this->shader);
    }

private:
    mr::Mesh *mesh = nullptr;
    mr::Shader *shader = nullptr;
    mr::FPSCamera cam;
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