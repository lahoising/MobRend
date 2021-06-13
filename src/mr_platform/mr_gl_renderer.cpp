#ifdef MOBREND_GL_RENDERING

#include <glad/glad.h>
#include <glm/glm.hpp>

#ifdef MOBREND_GLFW_WINDOW
#include "mr_platform/mr_glfw_window.h"
#endif

#include "mr_logger.h"
#include "mr_platform/mr_gl_renderer.h"
#include "mr_gui.h"
#include "mr_application.h"

#include "mr_model.h"
#include "mr_mesh.h"
#include "mr_shader.h"
#include "mr_camera.h"
#include "mr_vertex_layout.h"
#include "mr_texture.h"
#include "mr_observer_camera.h"
#include "mr_fps_camera.h"
#include "mr_light.h"

#include <assimp/Importer.hpp>

namespace mr
{

struct Renderer::gui_init_info_s
{
    char glVersion[16];
};
    
static Shader *shader = nullptr;

static FPSCamera cam;
static Texture *tex = nullptr;
static Texture *specMap = nullptr;

static Light *directional = new DirectionalLight(
    glm::vec3(1.0f, 1.0f, 1.0f), 
    0.5f);

static Model *model = nullptr;

GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);

    DirectionalLight *dirLight = (DirectionalLight*)directional;
    dirLight->direction = glm::normalize(glm::vec3(0.0f, -0.2f, 0.8f));

    Shader::CreateParams shaderCreateParams = {};
    shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\default_shader.vert.spv";
    shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\default_shader.frag.spv";
    shader = Shader::Create(shaderCreateParams);

    cam = FPSCamera();
    Camera::Config camConfig = {};
    camConfig.perspective.fov = glm::radians(60.0f);
    camConfig.perspective.aspectRatio = 1280.0f / 720.0f;
    camConfig.perspective.near = 0.1f;
    camConfig.perspective.far = 1000.0f;
    cam.camera = Camera(
        Camera::Type::PERSPECTIVE,
        camConfig
    );

    tex = Texture::Create("D:\\Pictures\\Screenshots\\Screenshot (44).png");
    specMap = Texture::Create("D:\\Documents\\progs\\krita_resources\\MobRend\\IU_Spec.png");

    Application::GetInstance().GetMainWindow()->SetCursorVisible(false);

    model = Model::Load("D:\\Documents\\Clases\\4th Year\\sem-2\\it386\\sprints\\3rd\\Lodge_Modular_Pieces_Exploded.fbx");
}

GlRenderer::~GlRenderer()
{
    delete(directional);
    delete(shader);
    delete(model);
}

void GlRenderer::SetViewport(int x, int y, int width, int height) 
{
    glViewport(x, y, width, height);
}

void GlRenderer::OnRenderBegin()
{
    static glm::mat4 identityMat = glm::identity<glm::mat4>();
    static glm::mat4 translated = glm::translate(identityMat, glm::vec3(1.0f, 0.5f, 10.0f));

    cam.Update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    Renderer::Command cmd = {};
    cmd.topologyType = TopologyType::TRIANGLES;
    cmd.model = model;
    cmd.renderObjectType = RenderObjectType::MODEL;
    this->Render(cmd);
}

void GlRenderer::OnRenderEnd()
{
    auto error = glGetError();
    if(error)
        printf("gl error %x\n", error);
}

void GlRenderer::Render(Renderer::Command &cmd)
{
    switch (cmd.renderObjectType)
    {
    case RenderObjectType::MESH:
    {
        const IndexBuffer *indexBuffer = cmd.mesh->GetIndexBuffer();
        cmd.mesh->GetVertexBuffer()->Bind();
        indexBuffer->Bind();
        glDrawElements(
            GlRenderer::GetTopology(cmd.topologyType),
            indexBuffer->GetElementCount(),
            GL_UNSIGNED_INT, nullptr
        );
    }
    break;
    
    case RenderObjectType::MODEL:
        cmd.renderObjectType = RenderObjectType::MESH;
        for(auto &mesh : cmd.model->GetMeshes())
        {
            cmd.mesh = mesh;
            this->Render(cmd);
        }
    break;

    default:
        mrwarn("Render cmd was called with no render object type");
    break;
    }
}

struct Renderer::gui_init_info_s *GlRenderer::GetGuiInitInfo()
{
    auto *guiInfo = (Renderer::gui_init_info_s*)malloc(sizeof(Renderer::gui_init_info_s));
    strcpy(guiInfo->glVersion, "#version 410");
    return guiInfo;
}

void GlRenderer::DeleteGuiInitInfo(Renderer::gui_init_info_s *info)
{
    delete(info);
}

GLenum GlRenderer::GetTopology(TopologyType type)
{
    switch (type)
    {
    case TopologyType::TRIANGLES: return GL_TRIANGLES;
    case TopologyType::WIREFRAME: return GL_LINE_STRIP;
    default: throw "Unsupported topology type";
    }
}

} // namespace mr


#endif