#include <iostream>
#include <unordered_map>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "mr_asset_manager.h"
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

        this->ambient = new mr::AmbientLight(
            glm::vec3(0.3f, 1.0f, 0.5f), 0.1f
        );

        mr::Shader::CreateParams shaderCreateParams = {};
        mr::AssetManager::GetAssetPath(shaderCreateParams.vertFilePath, "resources/shaders/default_shader.vert.spv");
        mr::AssetManager::GetAssetPath(shaderCreateParams.fragFilePath, "resources/shaders/default.frag.spv");
        shader = mr::Shader::Create(shaderCreateParams);

        shaderCreateParams = {};
        mr::AssetManager::GetAssetPath(shaderCreateParams.vertFilePath, "resources/shaders/skybox.vert.spv");
        mr::AssetManager::GetAssetPath(shaderCreateParams.fragFilePath, "resources/shaders/skybox.frag.spv");
        this->skyboxShader = mr::Shader::Create(shaderCreateParams);

        mr::AssetManager::GetAssetPath(shaderCreateParams.vertFilePath, "resources/shaders/quad.vert.spv");
        mr::AssetManager::GetAssetPath(shaderCreateParams.fragFilePath, "resources/shaders/gaussian.frag.spv");
        gaussianBlurShader = mr::Shader::Create(shaderCreateParams);

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
        // textureLoadParams.type = mr::Texture::TEXTURE_TYPE_2D;
        // textureLoadParams.invertVertically = false;
        // textureLoadParams.filepath = "D:\\Documents\\Clases\\4th Year\\sem-2\\it386\\sprints\\3rd\\Textures\\T_Bell_BaseColor.jpg";
        // tex = mr::Texture::Load(textureLoadParams);

        // textureLoadParams = {};
        // textureLoadParams.invertVertically = false;
        // textureLoadParams.filepath = "D:\\Documents\\Clases\\4th Year\\sem-2\\it386\\sprints\\3rd\\Textures\\T_Bell_OcclusionRoughnessMetallic.jpg";
        // this->lodgeSpecMap = mr::Texture::Load(textureLoadParams);
        
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

        char modelPath[MR_MAX_PATH];
        mr::AssetManager::GetAssetPath(modelPath, "resources/models/kunai.fbx");
        model = mr::Model::Load(modelPath);

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
        uboCreateParams.bufferSize = sizeof(glm::mat4) * 2;
        this->camUBO = mr::UniformBuffer::Create(uboCreateParams);
        this->shader->UploadUniformBuffer("CameraMatrices", this->camUBO);

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
        mr::Renderer::Command cmd = {};
        mr::FramebufferUsage framebufferUsage = mr::FRAMEBUFFER_USAGE_READ_WRITE;
        this->framebuffer->Bind(framebufferUsage);
        this->framebuffer->Clear(framebufferUsage);

        renderer->EnableRenderPass(
            mr::RENDER_PASS_DEPTH, true
        );

            this->RenderScene(renderer);

        this->framebuffer->Unbind(framebufferUsage);

        renderer->Clear();
        this->gaussianBlurShader->Bind();
        renderer->EnableRenderPass(
            mr::RENDER_PASS_DEPTH, false
        );

        this->gaussianBlurShader->UploadTexture("u_tex", this->framebuffer->GetTextureAttachment(0));
        
        cmd = {};
        cmd.mesh = this->screen;
        cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
        renderer->Render(cmd);
    }

    void RenderScene(mr::Renderer *renderer)
    {
        mr::Renderer::Command cmd = {};

        glm::mat4 identityMat = glm::identity<glm::mat4>();
        cam.Update();
        
        glm::mat4 normalMat = glm::mat3(glm::transpose(glm::inverse(identityMat)));
        float matrices[16 + 16] = {};
        memcpy(matrices, glm::value_ptr(cam.camera.GetViewProjection()), sizeof(glm::mat4));
        memcpy(matrices + 16, glm::value_ptr(normalMat), sizeof(glm::mat4));
        this->camUBO->SetData(matrices, sizeof(glm::mat4) * 2, 0);

        this->shader->Bind();
        this->shader->UploadMat4("u_model.model", identityMat);

        this->directional->Bind(this->shader, "u_scene.directional");
        this->ambient->Bind(this->shader, "u_scene.ambient");
        this->shader->UploadVec4("u_scene.material.diffuse", {1.0f, 0.5f, 0.2f, 1.0f});
        this->shader->UploadFloat("u_scene.material.diffuseMapStrength", 0.0f);
        this->shader->UploadVec4("u_scene.material.specular", {0.1f, 0.2f, 1.0f, 1.0f});
        this->shader->UploadFloat("u_scene.material.specularMapStrength", 0.0f);
        this->shader->UploadFloat("u_scene.material.shininess", 12.0f);
        this->shader->UploadVec3("u_scene.viewPos", this->cam.camera.GetPosition());

        cmd = {};
        cmd.model = this->model;
        cmd.renderObjectType = mr::RENDER_OBJECT_MODEL;
        renderer->Render(cmd);

        // glm::mat4 lodgeModelMatrix = glm::translate(identityMat, {0.0f, 0.0f, 10.0f});
        // normalMat = glm::mat3(glm::transpose(glm::inverse(lodgeModelMatrix)));
        // this->camUBO->SetData(glm::value_ptr(normalMat), sizeof(glm::mat4), sizeof(glm::mat4));
        
        // this->shader->UploadMat4("u_model.model", lodgeModelMatrix);
        // this->shader->UploadFloat("u_scene.material.diffuseMapStrength", 1.0f);
        // this->shader->UploadTexture("u_diffuseMap", this->tex);
        // this->shader->UploadFloat("u_scene.material.specularMapStrength", 1.0f);
        // this->shader->UploadTexture("u_specularMap", this->lodgeSpecMap);

        // cmd = {};
        // cmd.model = this->lodge;
        // cmd.renderObjectType = mr::RENDER_OBJECT_MODEL;
        // renderer->Render(cmd);

        this->RenderSkybox(renderer);
    }

    void RenderSkybox(mr::Renderer *renderer)
    {
        renderer->SetDepthTestFn(mr::RENDER_PASS_FN_LESS_OR_EQUEAL);
            this->skyboxShader->Bind();
            this->skyboxShader->UploadMat4(
                "u_cam.viewProjection", 
                cam.camera.GetProjectionMatrix() * glm::mat4(glm::mat3(cam.camera.GetViewMatrix()))
            );
            this->skyboxShader->UploadTexture("u_skybox", this->skyboxCubeMap);
            mr::Renderer::Command cmd = {};
            cmd.mesh = this->skybox;
            cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
            renderer->Render(cmd);
        renderer->SetDepthTestFn(mr::RENDER_PASS_FN_LESS);
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
        // delete(this->lodge);
        delete(this->skybox);
        delete(this->screen);
        delete(this->framebuffer);
        delete(this->directional);
        delete(this->ambient);
        delete(this->shader);
        delete(this->skyboxShader);
    }

private:
    mr::Shader *shader = nullptr;
    mr::Shader *skyboxShader = nullptr;
    mr::Shader *gaussianBlurShader = nullptr;
    mr::UniformBuffer *camUBO = nullptr;

    mr::FPSCamera cam;
    // mr::Texture *tex = nullptr;
    // mr::Texture *lodgeSpecMap = nullptr;
    mr::Texture *skyboxCubeMap = nullptr;
    mr::Framebuffer *framebuffer = nullptr;

    mr::DirectionalLight *directional = nullptr;
    mr::AmbientLight *ambient = nullptr;

    mr::Model *model = nullptr;
    // mr::Model *lodge = nullptr;
    mr::Mesh *skybox = nullptr;
    mr::Mesh *screen = nullptr;
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