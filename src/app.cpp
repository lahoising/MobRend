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

        shaderCreateParams = {};
        shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\quad.vert.spv";
        shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\quad.frag.spv";
        simpleQuad = mr::Shader::Create(shaderCreateParams);

        shaderCreateParams = {};
        shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\skybox.vert.spv";
        shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\shaders\\skybox.frag.spv";
        this->skyboxShader = mr::Shader::Create(shaderCreateParams);

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
        
        textureLoadParams.filepath = "D:\\Documents\\Art\\Sprites\\Exports\\semi_transparent_window.png";
        specMap = mr::Texture::Load(textureLoadParams);

        mr::Texture::CubePaths cubeMapPaths = {};
        cubeMapPaths.right = "D:\\Workspace\\Downloads\\skybox\\right.jpg";
        cubeMapPaths.left = "D:\\Workspace\\Downloads\\skybox\\left.jpg";
        cubeMapPaths.top = "D:\\Workspace\\Downloads\\skybox\\top.jpg";
        cubeMapPaths.bottom = "D:\\Workspace\\Downloads\\skybox\\bottom.jpg";
        cubeMapPaths.front = "D:\\Workspace\\Downloads\\skybox\\front.jpg";
        cubeMapPaths.back = "D:\\Workspace\\Downloads\\skybox\\back.jpg";
        
        textureLoadParams.cubeMapPaths = &cubeMapPaths;
        textureLoadParams.type = mr::Texture::TEXTURE_TYPE_CUBE;
        textureLoadParams.invertVertically = false;
        this->cubeMap = mr::Texture::Load(textureLoadParams);

        model = mr::Model::Load("D:\\Documents\\git\\MobRend\\resources\\models\\kunai.fbx");

        this->isCursonVisible = false;

        mr::VertexLayout layout = {
            {mr::AttributeType::FLOAT, 3},
            {mr::AttributeType::FLOAT, 3},
            {mr::AttributeType::FLOAT, 2},
        };

        float quadVertices[] = {
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f
        };

        uint32_t quadIndices[] = {
            0, 1, 2, 2, 3, 0
        };

        mr::Mesh::CreateParams meshCreateParams = {};
        meshCreateParams.vertexLayout = &layout;
        meshCreateParams.indices = quadIndices;
        meshCreateParams.indexCount = sizeof(quadIndices) / sizeof(uint32_t);
        meshCreateParams.vertices = quadVertices;
        meshCreateParams.verticesArraySize = sizeof(quadVertices);
        
        this->quad = new mr::Mesh(meshCreateParams);

        layout = {
            {mr::AttributeType::FLOAT, 2},
            {mr::AttributeType::FLOAT, 2},
        };

        float screenVertices[] = {
             0.9f, -0.9f, 1.0f, 0.0f,
             0.9f,  0.9f, 1.0f, 1.0f,
            -0.9f,  0.9f, 0.0f, 1.0f,
            -0.9f, -0.9f, 0.0f, 0.0f
        };

        meshCreateParams.vertexLayout = &layout;
        meshCreateParams.vertices = screenVertices;
        meshCreateParams.verticesArraySize = sizeof(screenVertices);
        this->screen = new mr::Mesh(meshCreateParams);

        layout = {
            {mr::AttributeType::FLOAT, 3}
        };

        float skyboxVertices[] = {
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
        };

        uint32_t skyboxIndices[] = {
            0, 1, 2, 2, 3, 0,   // front
            4, 5, 6, 6, 7, 4,   // back
            7, 6, 1, 1, 0, 7,   // right
            3, 2, 5, 5, 4, 3,   // left
            1, 6, 5, 5, 2, 1,   // top
            7, 0, 3, 3, 4, 7    // bottom 
        };

        meshCreateParams = {};
        meshCreateParams.vertexLayout = &layout;
        meshCreateParams.vertices = skyboxVertices;
        meshCreateParams.verticesArraySize = sizeof(skyboxVertices);
        meshCreateParams.indices = skyboxIndices;
        meshCreateParams.indexCount = sizeof(skyboxIndices) / sizeof(uint32_t);
        this->skybox = new mr::Mesh(meshCreateParams);

        mr::Application &app = mr::Application::GetInstance();
        mr::Window *window = app.GetMainWindow();
        window->SetCursorVisible(isCursonVisible);

        mr::Renderer *rend = app.GetRenderer();

        mr::Framebuffer::CreateParams frameBufferCreateParams = {};
        frameBufferCreateParams.width = window->GetFramebufferWidth();
        frameBufferCreateParams.height = window->GetFramebufferHeight();
        frameBufferCreateParams.attachments = {
            { mr::Framebuffer::ATTACHMENT_COLOR_0, false },
            { mr::Framebuffer::ATTACHMENT_DEPTH24_STENCIL8, true }
        };
        this->framebuffer = mr::Framebuffer::Create(frameBufferCreateParams);
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
        mr::FramebufferUsage framebufferUsage = mr::FRAMEBUFFER_USAGE_READ_WRITE;
        this->framebuffer->Bind(framebufferUsage);
        this->framebuffer->Clear(framebufferUsage);

        renderer->EnableRenderPass(
            mr::RENDER_PASS_DEPTH, true
        );

        glm::mat4 identityMat = glm::identity<glm::mat4>();
        cam.Update();

        renderer->SetDepthMask(false);
            this->skyboxShader->Bind();
            this->skyboxShader->UploadMat4(
                "u_cam.viewProjection", 
                cam.camera.GetProjectionMatrix() * glm::mat4(glm::mat3(cam.camera.GetViewMatrix()))
            );
            this->skyboxShader->UploadTexture("u_skybox", this->cubeMap);
            mr::Renderer::Command cmd = {};
            cmd.mesh = this->skybox;
            cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
            renderer->Render(cmd);
        renderer->SetDepthMask(true);

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

        shader->UploadVec4(
            "u_scene.color",
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        );

        directional->Bind(shader, "u_scene.ambientLight");
        
        shader->UploadVec3(
            "u_scene.viewPos",
            cam.camera.GetPosition()
        );

        shader->UploadVec4("u_scene.phongMaterial.diffuse", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
        shader->UploadFloat("u_scene.phongMaterial.diffuseMapStrength", 0.0f);
        
        shader->UploadVec4("u_scene.phongMaterial.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.0f));
        shader->UploadFloat("u_scene.phongMaterial.specularMapStrength", 0.0f);
        shader->UploadFloat("u_scene.phongMaterial.shininess", 32.f);

        shader->UploadTexture("u_diffuseMap", tex);
        shader->UploadTexture("u_specularMap", specMap);

        cmd = {};
        cmd.topologyType = mr::TOPOLOGY_TRIANGLES;
        cmd.model = model;
        cmd.renderObjectType = mr::RENDER_OBJECT_MODEL;
        renderer->Render(cmd);

        shader->UploadFloat("u_scene.phongMaterial.diffuseMapStrength", 1.0f);

        cmd = {};
        cmd.topologyType = mr::TOPOLOGY_TRIANGLES;
        cmd.mesh = this->quad;
        cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
        renderer->Render(cmd);

        shader->UploadTexture("u_diffuseMap", specMap);
        // shader->UploadFloat("u_scene.phongMaterial.diffuseMapStrength", 0.0f);
        shader->UploadMat4("u_cam.model", glm::translate(
            identityMat, glm::vec3(0.0f, 0.0f, -2.5f)
        ));

        renderer->Render(cmd);

        this->framebuffer->Unbind(framebufferUsage);

        renderer->Clear();
        this->simpleQuad->Bind();
        renderer->EnableRenderPass(
            mr::RENDER_PASS_DEPTH, false
        );

        this->simpleQuad->UploadTexture("u_tex", this->framebuffer->GetTextureAttachment(0));
        
        cmd = {};
        // cmd.topologyType = mr::TOPOLOGY_WIREFRAME;
        cmd.topologyType = mr::TOPOLOGY_TRIANGLES;
        cmd.mesh = this->screen;
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
        if(this->framebuffer) delete(this->framebuffer);
        delete(this->quad);
        delete(this->model);
        delete(this->skybox);
        delete(this->directional);
        delete(this->shader);
        delete(this->simpleQuad);
        delete(this->skyboxShader);
    }

private:
    mr::Framebuffer *framebuffer = nullptr;
    mr::Shader *shader = nullptr;
    mr::Shader *simpleQuad = nullptr;
    mr::Shader *skyboxShader = nullptr;

    mr::FPSCamera cam;
    mr::Texture *tex = nullptr;
    mr::Texture *specMap = nullptr;
    mr::Texture *cubeMap = nullptr;

    mr::DirectionalLight *directional = nullptr;

    mr::Model *model = nullptr;
    mr::Mesh *quad = nullptr;
    mr::Mesh *screen = nullptr;
    mr::Mesh *skybox = nullptr;
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