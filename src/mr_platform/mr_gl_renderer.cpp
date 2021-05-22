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

namespace mr
{

struct Renderer::gui_init_info_s
{
    char glVersion[16];
};
    
static GLuint vertexArrayId;
static GLuint indexBufferId;
static Buffer *vertexBuffer = nullptr;
static Shader *shader = nullptr;

GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glClearColor(0.8f, 0.3f, 0.2f, 1.0f);

    Shader::CreateParams shaderCreateParams = {};
    shaderCreateParams.vertFilePath = "vert";
    shaderCreateParams.fragFilePath = "frag";
    shader = Shader::Create(shaderCreateParams);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    Buffer::CreateParams bufferCreateParams = {};
    bufferCreateParams.type = Buffer::Type::VERTEX;
    bufferCreateParams.size = sizeof(g_vertex_buffer_data);
    bufferCreateParams.data = (void*)g_vertex_buffer_data;
    vertexBuffer = Buffer::Create(bufferCreateParams);

    const uint32_t indices[] = {0, 1, 2};
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glm::vec3 vec = {1.0f, 0.0f, 0.0f};
    printf("vec: %.2f, %.2f, %.2f\n", vec.x, vec.y, vec.z);
}

GlRenderer::~GlRenderer()
{
    delete(vertexBuffer);
    glDeleteBuffers(1, &indexBufferId);
    glDeleteVertexArrays(1, &vertexArrayId);
    delete(shader);
}

void GlRenderer::SetViewport(int x, int y, int width, int height) 
{
    glViewport(x, y, width, height);
}

void GlRenderer::OnRenderBegin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Bind();

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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