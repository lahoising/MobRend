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

#include "mr_vertex_buffer.h"
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
static VertexBuffer *vertexBuffer = nullptr;
static unsigned int indexBufferId = 0;
static unsigned int indexCount = 0;
// static Buffer *indexBuffer = nullptr;

static FPSCamera cam;
static Texture *tex = nullptr;
static Texture *specMap = nullptr;

static Shader *lightSourceShader = nullptr;
static VertexBuffer *lightSourceVertexBuffer = nullptr;
static unsigned int lightSourceIndexBufferId = 0;
static unsigned int lightSourceIndexCount = 0;
// static Buffer *lightSourceIndexBuffer = nullptr;

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
    // shaderCreateParams.vertFilePath = "";
    // shaderCreateParams.fragFilePath = "";
    shader = Shader::Create(shaderCreateParams);

    shaderCreateParams.vertFilePath = "D:\\Documents\\git\\MobRend\\resources\\lightsource.vert";
    shaderCreateParams.fragFilePath = "D:\\Documents\\git\\MobRend\\resources\\lightsource.frag";
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

    VertexBuffer::CreateParams vertexBuffCreateParams = {};
    vertexBuffCreateParams.bufferSize = sizeof(g_vertex_buffer_data);
    vertexBuffCreateParams.data = (void*)g_vertex_buffer_data;
    vertexBuffCreateParams.vertexLayout = &layout;
    vertexBuffer = VertexBuffer::Create(vertexBuffCreateParams);

    const uint32_t indices[] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
    };
    // indexBuffer = Buffer::Create(vertexBuffCreateParams);
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    indexCount = sizeof(indices) / sizeof(uint32_t);

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

    vertexBuffCreateParams = {};
    vertexBuffCreateParams.bufferSize = sizeof(g_light_source_vertices);
    vertexBuffCreateParams.data = (void*)g_light_source_vertices;
    vertexBuffCreateParams.vertexLayout = &lightSourceVertexLayout;
    lightSourceVertexBuffer = VertexBuffer::Create(vertexBuffCreateParams);

    const uint32_t lightSourceindices[] = {
        0, 1, 2, 
        0, 2, 3,
        0, 3, 4,
        0, 4, 1
    };
    glGenBuffers(1, &lightSourceIndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSourceIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightSourceindices), lightSourceindices, GL_STATIC_DRAW);
    lightSourceIndexCount = sizeof(lightSourceindices) / sizeof(uint32_t);
    // lightSourceIndexBuffer = Buffer::Create(vertexBuffCreateParams);

    // cam = ObserverCamera();
    cam = FPSCamera();
    Camera::Config camConfig = {};
    camConfig.perspective.fov = glm::radians(60.0f);
    // camConfig.ortho.size = 3.0f;
    camConfig.perspective.aspectRatio = 1280.0f / 720.0f;
    camConfig.perspective.near = 0.1f;
    camConfig.perspective.far = 100.0f;
    cam.camera = Camera(
        Camera::Type::PERSPECTIVE,
        camConfig
    );


    tex = Texture::Create("D:\\Pictures\\Screenshots\\Screenshot (44).png");
    specMap = Texture::Create("D:\\Documents\\progs\\krita_resources\\MobRend\\IU_Spec.png");

    vertexBuffer->Bind();    
    shader->Bind();
    shader->UploadInt("u_phongMaterial.diffuseMap", 0);
    shader->UploadInt("u_phongMaterial.specularMap", 1);

    Application::GetInstance().GetMainWindow()->SetCursorVisible(false);
}

GlRenderer::~GlRenderer()
{
    delete(ambient);
    delete(point);
    delete(directionalLight);
    delete(spotlight);

    delete(specMap);
    delete(tex);

    glDeleteBuffers(1, &indexBufferId);
    // delete(indexBuffer);
    delete(vertexBuffer);
    delete(shader);
    
    glDeleteBuffers(1, &lightSourceIndexBufferId);
    // delete(lightSourceIndexBuffer);
    delete(lightSourceVertexBuffer);
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
        "u_viewProjection",
        cam.camera.GetViewProjection()
    );

    shader->UploadMat4(
        "u_model",
        identityMat
    );

    glm::mat3 normalMat = glm::transpose(glm::inverse(identityMat));
    shader->UploadMat3(
        "u_normalMat",
        normalMat
    );

    shader->UploadVec3(
        "u_color",
        glm::vec3(1.0f, 1.0f, 1.0f)
    );

    // ambient->Bind(shader, "u_ambientLight");
    point->Bind(shader, "u_pointLight");
    
    // DirectionalLight *dirLight = (DirectionalLight*)directionalLight;
    // dirLight->direction =   glm::quat(glm::vec3(0.0f, glm::radians(0.5f), 0.0f)) * 
    //                         dirLight->direction;
    // directionalLight->Bind(shader, "u_ambientLight");

    Spotlight *spot = (Spotlight*)spotlight;
    spot->position = cam.camera.GetPosition();
    spot->rotation = cam.camera.GetRotation();
    spotlight->Bind(shader, "u_ambientLight");

    shader->UploadVec3(
        "u_viewPos",
        cam.camera.GetPosition()
    );

    shader->UploadVec3("u_phongMaterial.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    shader->UploadFloat("u_phongMaterial.diffuseMapStrength", 1.0f);
    
    shader->UploadVec3("u_phongMaterial.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->UploadFloat("u_phongMaterial.specularMapStrength", 1.0f);
    shader->UploadFloat("u_phongMaterial.shininess", 32.f);

    glActiveTexture(GL_TEXTURE0);
    tex->Bind();

    glActiveTexture(GL_TEXTURE0 + 1);
    specMap->Bind();

    vertexBuffer->Bind();
    // indexBuffer->Bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glDrawElements(
        GL_TRIANGLES, 
        indexCount, 
        GL_UNSIGNED_INT, nullptr);

    shader->UploadMat4(
        "u_model",
        translated
    );
    shader->UploadVec3(
        "u_color",
        glm::vec3(1.0f, 1.0f, 1.0f)
    );

    glDrawElements(
        GL_TRIANGLES, 
        indexCount, 
        GL_UNSIGNED_INT, nullptr);

    const glm::mat4 lightSourceScaleMat = glm::scale(identityMat, glm::vec3(0.2f, 0.2f, 0.2f));
    lightSourceShader->Bind();
    lightSourceShader->UploadMat4(
        "u_viewProjection",
        cam.camera.GetViewProjection());
    lightSourceShader->UploadMat4(
        "u_model",
        glm::translate(lightSourceScaleMat, point->position));

    lightSourceShader->UploadVec3(
        "u_lightColor",
        point->color
    );

    lightSourceVertexBuffer->Bind();
    // lightSourceIndexBuffer->Bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSourceIndexBufferId);
    glDrawElements(
        GL_TRIANGLES,
        lightSourceIndexCount,
        GL_UNSIGNED_INT, nullptr
    );

    auto error = glGetError();
    if(error)
        printf("gl error %x\n", error);
}

void GlRenderer::OnRenderEnd()
{
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

} // namespace mr


#endif