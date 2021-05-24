#ifdef MOBREND_GL_RENDERING

#include <glad/glad.h>
#include <glm/glm.hpp>

#ifdef MOBREND_GLFW_WINDOW
#include "mr_platform/mr_glfw_window.h"
#endif

#include "mr_platform/mr_gl_renderer.h"
#include "mr_gui.h"
#include "mr_application.h"

#include "mr_buffer.h"
#include "mr_shader.h"
#include "mr_camera.h"

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
static Camera *cam = nullptr;

GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);

    Shader::CreateParams shaderCreateParams = {};
    // shaderCreateParams.vertFilePath = "";
    // shaderCreateParams.fragFilePath = "";
    shader = Shader::Create(shaderCreateParams);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    Buffer::CreateParams bufferCreateParams = {};
    bufferCreateParams.type = Buffer::Type::VERTEX;
    bufferCreateParams.size = sizeof(g_vertex_buffer_data);
    bufferCreateParams.data = (void*)g_vertex_buffer_data;
    vertexBuffer = Buffer::Create(bufferCreateParams);

    const uint32_t indices[] = {0, 1, 2};
    bufferCreateParams.type = Buffer::Type::INDEX;
    bufferCreateParams.size = sizeof(indices);
    bufferCreateParams.data = (void*)indices;
    indexBuffer = Buffer::Create(bufferCreateParams);

    Camera::Config camConfig = {};
    camConfig.ortho.size = 1.0f;
    camConfig.ortho.aspectRatio = 1280.0f / 720.0f;
    camConfig.ortho.near = 0.01f;
    camConfig.ortho.far = 100.0f;
    cam = new Camera(
        Camera::Type::ORTHOGRAPHIC,
        camConfig
    );
}

GlRenderer::~GlRenderer()
{
    delete(vertexBuffer);
    delete(indexBuffer);
    glDeleteVertexArrays(1, &vertexArrayId);
    delete(shader);
    delete(cam);
}

void GlRenderer::SetViewport(int x, int y, int width, int height) 
{
    glViewport(x, y, width, height);
}

void GlRenderer::OnRenderBegin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Bind();

    shader->UploadMat4(
        "viewProjection",
        cam->GetViewProjection()
    );

    glEnableVertexAttribArray(0);
    vertexBuffer->Bind();
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );

    indexBuffer->Bind();
    glDrawElements(
        GL_TRIANGLES, 
        indexBuffer->GetElementCount(), 
        GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(0);

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