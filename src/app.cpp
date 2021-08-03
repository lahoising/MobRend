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
#include "mr_texture.h"
#include "mr_framebuffer.h"

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

        shaderParams = {};
        mr::AssetManager::GetAssetPath(shaderParams.vertFilePath, "resources/shaders/skybox.vert.spv");
        mr::AssetManager::GetAssetPath(shaderParams.fragFilePath, "resources/shaders/skybox.frag.spv");
        this->skyboxShader = mr::Shader::Create(shaderParams);

        mr::Texture::LoadParams textureParams = {};
        textureParams.filepath = "D:\\Documents\\Clases\\4th Year\\sem-2\\it386\\sprints\\3rd\\Textures\\Lodge_Modular_Pieces_Exploded_Lodge_Walls_Set_BaseColor.jpg";
        textureParams.invertVertically = false;
        this->diffuseTexture = mr::Texture::Load(textureParams);

        mr::VertexLayout layout = {
            {mr::ATTRIBUTE_TYPE_FLOAT, 3}
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

        mr::Mesh::CreateParams meshCreateParams = {};
        meshCreateParams.vertexLayout = &layout;
        meshCreateParams.vertices = skyboxVertices;
        meshCreateParams.verticesArraySize = sizeof(skyboxVertices);
        meshCreateParams.indices = skyboxIndices;
        meshCreateParams.indexCount = sizeof(skyboxIndices) / sizeof(uint32_t);
        this->skybox = new mr::Mesh(meshCreateParams);

        mr::Texture::LoadParams textureLoadParams = {};
        mr::Texture::CubePaths cubeMapPaths = {};
        mr::AssetManager::GetAssetPath(cubeMapPaths.right,  "resources/images/skybox/LearnOpenGl/right.jpg");
        mr::AssetManager::GetAssetPath(cubeMapPaths.left,   "resources/images/skybox/LearnOpenGl/left.jpg");
        mr::AssetManager::GetAssetPath(cubeMapPaths.top,    "resources/images/skybox/LearnOpenGl/top.jpg");
        mr::AssetManager::GetAssetPath(cubeMapPaths.bottom, "resources/images/skybox/LearnOpenGl/bottom.jpg");
        mr::AssetManager::GetAssetPath(cubeMapPaths.front,  "resources/images/skybox/LearnOpenGl/front.jpg");
        mr::AssetManager::GetAssetPath(cubeMapPaths.back,   "resources/images/skybox/LearnOpenGl/back.jpg");
        
        textureLoadParams.cubeMapPaths = &cubeMapPaths;
        textureLoadParams.type = mr::Texture::TEXTURE_TYPE_CUBE;
        textureLoadParams.invertVertically = false;
        this->skyboxCubeMap = mr::Texture::Load(textureLoadParams);

        shaderParams = {};
        mr::AssetManager::GetAssetPath(shaderParams.vertFilePath, "resources/shaders/quad.vert.spv");
        mr::AssetManager::GetAssetPath(shaderParams.fragFilePath, "resources/shaders/gaussian.frag.spv");
        gaussianBlurShader = mr::Shader::Create(shaderParams);

        layout = {
            {mr::ATTRIBUTE_TYPE_FLOAT, 2},
            {mr::ATTRIBUTE_TYPE_FLOAT, 2},
        };

        float screenVertices[] = {
             0.9f, -0.9f, 1.0f, 0.0f,
             0.9f,  0.9f, 1.0f, 1.0f,
            -0.9f,  0.9f, 0.0f, 1.0f,
            -0.9f, -0.9f, 0.0f, 0.0f
        };

        uint32_t quadIndices[] = {
            0, 1, 2, 2, 3, 0
        };

        meshCreateParams.vertexLayout = &layout;
        meshCreateParams.vertices = screenVertices;
        meshCreateParams.verticesArraySize = sizeof(screenVertices);
        meshCreateParams.indices = quadIndices;
        meshCreateParams.indexCount = sizeof(quadIndices) / sizeof(uint32_t);
        this->screen = new mr::Mesh(meshCreateParams);

        mr::Window *window = mr::Application::GetInstance().GetMainWindow();
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
        mr::FramebufferUsage framebufferUsage = mr::FRAMEBUFFER_USAGE_READ_WRITE;
        this->framebuffer->Bind(framebufferUsage);
        this->framebuffer->Clear(framebufferUsage);

        renderer->EnableRenderPass(
            mr::RENDER_PASS_DEPTH, true
        );

        RenderScene(renderer);

        this->framebuffer->Unbind(framebufferUsage);

        renderer->Clear();
        this->gaussianBlurShader->Bind();
        renderer->EnableRenderPass(
            mr::RENDER_PASS_DEPTH, false
        );

        this->gaussianBlurShader->UploadTexture("u_tex", this->framebuffer->GetTextureAttachment(0));

        mr::Renderer::Command cmd = {};
        cmd.mesh = this->screen;
        cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
        renderer->Render(cmd);
    }

    void RenderScene(mr::Renderer *renderer)
    {
        glm::mat4 identityMat = glm::identity<glm::mat4>();
        this->shader->Bind();
        this->light->Bind(this->shader, "u_scene.directional");
        this->shader->UploadVec4("u_scene.material.diffuse", {1.0f, 0.5f, 0.2f, 1.0f});
        this->shader->UploadFloat("u_scene.material.diffuseMapStrength", 1.0f);
        this->shader->UploadVec4("u_scene.material.specular", {0.1f, 0.2f, 1.0f, 1.0f});
        this->shader->UploadFloat("u_scene.material.specularMapStrength", 0.0f);
        this->shader->UploadFloat("u_scene.material.shininess", 12.0f);
        this->shader->UploadVec3("u_scene.viewPos", this->cam.camera.GetPosition());

        this->shader->UploadTexture("u_diffuseMap", this->diffuseTexture);

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

        renderer->SetDepthTestFn(mr::RENDER_PASS_FN_LESS_OR_EQUEAL);
            this->skyboxShader->Bind();
            this->skyboxShader->UploadMat4(
                "u_cam.viewProjection", 
                cam.camera.GetProjectionMatrix() * glm::mat4(glm::mat3(cam.camera.GetViewMatrix()))
            );
            this->skyboxShader->UploadTexture("u_skybox", this->skyboxCubeMap);
            cmd = {};
            cmd.mesh = this->skybox;
            cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
            renderer->Render(cmd);
        renderer->SetDepthTestFn(mr::RENDER_PASS_FN_LESS);

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
        delete(this->skyboxShader);
        delete(this->skybox);
        delete(this->framebuffer);
        delete(this->screen);
        delete(this->gaussianBlurShader);
    }

private:
    mr::Model *model = nullptr;
    mr::Shader *shader = nullptr;
    mr::Light *light = nullptr;
    mr::FPSCamera cam;
    mr::UniformBuffer *camUBO = nullptr;
    mr::Texture *diffuseTexture = nullptr;
    mr::Shader *skyboxShader = nullptr;
    mr::Texture *skyboxCubeMap = nullptr;
    mr::Mesh *skybox = nullptr;
    mr::Framebuffer *framebuffer = nullptr;
    mr::Shader *gaussianBlurShader = nullptr;
    mr::Mesh *screen = nullptr;
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