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
static Mesh *cube = nullptr;

static FPSCamera cam;
static Texture *tex = nullptr;
static Texture *specMap = nullptr;

static Shader *lightSourceShader = nullptr;
static Mesh *pyramid = nullptr;

static Light *ambient = new AmbientLight(
    glm::vec3(1.0f, 1.0f, 1.0f), 
    0.5f);

static Light *point = new PointLight(
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f
);

static Light *directionalLight = new DirectionalLight(
    glm::vec3(1.0f, 1.0f, 1.0f),
    0.8f
);

static Light *spotlight = new Spotlight(
    glm::vec3(1.0f, 1.0f, 1.0f), 1.0f
);

static Model *model = nullptr;

GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);

    point->position = glm::vec3(0.0f, 0.0f, 2.0f);

    DirectionalLight *dirLight = (DirectionalLight*)directionalLight;
    dirLight->direction = glm::vec3(0.0f, -0.5f, 0.5f);

    spotlight->position = glm::vec3(0.0f, 0.0f, -3.0f);

    Shader::CreateParams shaderCreateParams = {};
    shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\default_shader.vert.spv";
    shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\default_shader.frag.spv";
    shader = Shader::Create(shaderCreateParams);

    shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\lightsource.vert.spv";
    shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\lightsource.frag.spv";
    lightSourceShader = Shader::Create(shaderCreateParams);

    const GLfloat g_vertex_buffer_data[] = {
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,

         0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
         
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
    };

    VertexLayout layout = VertexLayout({
        {AttributeType::FLOAT, 3},
        {AttributeType::FLOAT, 3},
        {AttributeType::FLOAT, 2}
    });

    const uint32_t indices[] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
    };
    
    Mesh::CreateParams meshCreateParams = {};
    meshCreateParams.vertices = g_vertex_buffer_data;
    meshCreateParams.verticesArraySize = sizeof(g_vertex_buffer_data);
    meshCreateParams.vertexLayout = &layout;
    meshCreateParams.indices = indices;
    meshCreateParams.indexCount = sizeof(indices) / sizeof(uint32_t);
    cube = new Mesh(meshCreateParams);
    
    const GLfloat g_light_source_vertices[] = {
         0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f
    };

    VertexLayout lightSourceVertexLayout = VertexLayout({
        {AttributeType::FLOAT, 3}
    });

    const uint32_t lightSourceindices[] = {
        0, 1, 2, 
        0, 2, 3,
        0, 3, 4,
        0, 4, 1
    };

    meshCreateParams = {};
    meshCreateParams.vertices = g_light_source_vertices;
    meshCreateParams.verticesArraySize = sizeof(g_light_source_vertices);
    meshCreateParams.vertexLayout = &lightSourceVertexLayout;
    meshCreateParams.indices = lightSourceindices;
    meshCreateParams.indexCount = sizeof(lightSourceindices) / sizeof(uint32_t);
    pyramid = new Mesh(meshCreateParams);

    // cam = ObserverCamera();
    cam = FPSCamera();
    Camera::Config camConfig = {};
    camConfig.perspective.fov = glm::radians(60.0f);
    // camConfig.ortho.size = 3.0f;
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
    delete(ambient);
    delete(point);
    delete(directionalLight);
    delete(spotlight);

    delete(specMap);
    delete(tex);

    delete(cube);
    delete(shader);
    delete(model);
    
    delete(pyramid);
    delete(lightSourceShader);
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

    // ambient->Bind(shader, "u_ambientLight");
    point->Bind(shader, "u_scene.pointLight");
    
    // DirectionalLight *dirLight = (DirectionalLight*)directionalLight;
    // dirLight->direction =   glm::quat(glm::vec3(0.0f, glm::radians(0.5f), 0.0f)) * 
    //                         dirLight->direction;
    // directionalLight->Bind(shader, "u_ambientLight");

    Spotlight *spot = (Spotlight*)spotlight;
    spot->position = cam.camera.GetPosition();
    spot->rotation = cam.camera.GetRotation();
    spotlight->Bind(shader, "u_scene.ambientLight");

    shader->UploadVec3(
        "u_scene.viewPos",
        cam.camera.GetPosition()
    );

    shader->UploadVec3("u_scene.phongMaterial.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    shader->UploadFloat("u_scene.phongMaterial.diffuseMapStrength", 1.0f);
    
    shader->UploadVec3("u_scene.phongMaterial.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->UploadFloat("u_scene.phongMaterial.specularMapStrength", 1.0f);
    shader->UploadFloat("u_scene.phongMaterial.shininess", 32.f);

    shader->UploadTexture2D("u_diffuseMap", tex);
    shader->UploadTexture2D("u_specularMap", specMap);

    Renderer::Command cmd = {};
    cmd.mesh = cube;
    cmd.renderObjectType = RenderObjectType::MESH;
    this->Render(cmd);

    shader->UploadFloat("u_scene.phongMaterial.diffuseMapStrength", 0.0f);
    shader->UploadFloat("u_scene.phongMaterial.specularMapStrength", 0.0f);
    cmd.model = model;
    cmd.renderObjectType = RenderObjectType::MODEL;
    this->Render(cmd);

    shader->UploadMat4(
        "u_cam.model",
        translated
    );
    shader->UploadVec3(
        "u_cam.color",
        glm::vec3(1.0f, 1.0f, 1.0f)
    );

    cmd.topologyType = TopologyType::WIREFRAME;
    cmd.renderObjectType = RenderObjectType::MESH;
    cmd.mesh = cube;
    this->Render(cmd);

    const glm::mat4 lightSourceScaleMat = glm::scale(identityMat, glm::vec3(0.2f, 0.2f, 0.2f));
    lightSourceShader->Bind();
    lightSourceShader->UploadMat4(
        "cameraConstants.u_viewProjection",
        cam.camera.GetViewProjection());
    lightSourceShader->UploadMat4(
        "cameraConstants.u_model",
        glm::translate(lightSourceScaleMat, point->position));

    lightSourceShader->UploadVec3(
        "lightConstants.u_lightColor",
        point->color
    );

    cmd = {};
    cmd.mesh = pyramid;
    this->Render(cmd);

    auto error = glGetError();
    if(error)
        printf("gl error %x\n", error);
}

void GlRenderer::OnRenderEnd()
{
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