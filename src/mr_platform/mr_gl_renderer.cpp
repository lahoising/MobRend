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

#include "mr_buffer.h"
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
    
static GLuint vertexArrayId;
static Buffer *vertexBuffer = nullptr;
static Buffer *indexBuffer = nullptr;
static Shader *shader = nullptr;
static FPSCamera cam;
static Texture *tex = nullptr;
static Texture *specMap = nullptr;

static Shader *lightSourceShader = nullptr;
static GLuint lightSourceVAO;
static Buffer *lightSourceVertexBuffer = nullptr;
static Buffer *lightSourceIndexBuffer = nullptr;

static Light *ambient = new AmbientLight(
    glm::vec3(1.0f, 1.0f, 1.0f), 
    1.0f);

static Light *point = new PointLight(
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f
);

static Light *directionalLight = new DirectionalLight(
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f
);

static Light *spotlight = new Spotlight(
    glm::vec3(1.0f, 1.0f, 1.0f), 1.0f
);

static GLenum GetAttribType(AttributeType type)
{
    switch (type)
    {
    case AttributeType::FLOAT: return GL_FLOAT;
    default: throw "Unknown attribute type";
    }
}

GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);

    ambient->intensity = 0.3f;
    
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

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

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

    Buffer::CreateParams bufferCreateParams = {};
    bufferCreateParams.type = Buffer::Type::VERTEX;
    bufferCreateParams.size = sizeof(g_vertex_buffer_data);
    bufferCreateParams.data = (void*)g_vertex_buffer_data;
    vertexBuffer = Buffer::Create(bufferCreateParams);

    const uint32_t indices[] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
    };
    bufferCreateParams.type = Buffer::Type::INDEX;
    bufferCreateParams.size = sizeof(indices);
    bufferCreateParams.data = (void*)indices;
    indexBuffer = Buffer::Create(bufferCreateParams);

    VertexLayout layout = VertexLayout({
        {AttributeType::FLOAT, 3},
        {AttributeType::FLOAT, 3},
        {AttributeType::FLOAT, 2}
    });
    this->DefineVertexLayout(layout);


    glGenVertexArrays(1, &lightSourceVAO);
    glBindVertexArray(lightSourceVAO);

    const GLfloat g_light_source_vertices[] = {
         0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f
    };

    bufferCreateParams = {};
    bufferCreateParams.type = Buffer::Type::VERTEX;
    bufferCreateParams.size = sizeof(g_light_source_vertices);
    bufferCreateParams.data = (void*)g_light_source_vertices;
    lightSourceVertexBuffer = Buffer::Create(bufferCreateParams);

    const uint32_t lightSourceindices[] = {
        0, 1, 2, 
        0, 2, 3,
        0, 3, 4,
        0, 4, 1
    };
    bufferCreateParams.type = Buffer::Type::INDEX;
    bufferCreateParams.size = sizeof(lightSourceindices);
    bufferCreateParams.data = (void*)lightSourceindices;
    lightSourceIndexBuffer = Buffer::Create(bufferCreateParams);

    VertexLayout lightSourceVertexLayout = VertexLayout({
        {AttributeType::FLOAT, 3}
    });
    this->DefineVertexLayout(lightSourceVertexLayout);

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
    glBindVertexArray(vertexArrayId);
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

    delete(vertexBuffer);
    delete(indexBuffer);
    glDeleteVertexArrays(1, &vertexArrayId);
    delete(shader);
    
    delete(lightSourceVertexBuffer);
    delete(lightSourceIndexBuffer);
    glDeleteVertexArrays(1, &lightSourceVAO);
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

    glBindVertexArray(vertexArrayId);
    vertexBuffer->Bind();
    indexBuffer->Bind();
    glDrawElements(
        GL_TRIANGLES, 
        indexBuffer->GetElementCount(), 
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
        indexBuffer->GetElementCount(), 
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

    glBindVertexArray(lightSourceVAO);
    lightSourceVertexBuffer->Bind();
    lightSourceIndexBuffer->Bind();
    glDrawElements(
        GL_TRIANGLES,
        lightSourceIndexBuffer->GetElementCount(),
        GL_UNSIGNED_INT, nullptr
    );

    auto error = glGetError();
    if(error)
        printf("gl error %x\n", error);
}

void GlRenderer::OnRenderEnd()
{
}

void GlRenderer::DefineVertexLayout(const VertexLayout &layout)
{
    int i = 0;
    uint32_t offset = 0;
    const std::vector<Attribute> &attributes = layout.GetAttributes();
    for(auto attrib : attributes)
    {
        uint32_t attribSize = VertexLayout::GetAttributeSize(attrib);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i, 
            attrib.count,
            GetAttribType(attrib.type),
            GL_FALSE,
            layout.GetStride(),
            (const void *)((size_t)offset)
        );
        offset += attribSize;
        i++;
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

} // namespace mr


#endif