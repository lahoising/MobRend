#include <iostream>
#include "mr_application.h"
#include "mr_mesh.h"

class UserApp : public mr::Program
{
public:
    UserApp(){}
    ~UserApp(){}

    virtual void OnStart() override
    {
        mr::VertexLayout layout = 
        {
            {mr::ATTRIBUTE_TYPE_FLOAT, 2}
        };

        float vertices[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f, 0.5f
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
    }

    virtual void OnUpdate() override
    {
    }

    virtual void OnRender(mr::Renderer *renderer) override
    {
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
    }

private:
    mr::Mesh *mesh = nullptr;
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