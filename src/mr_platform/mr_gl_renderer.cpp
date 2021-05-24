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
    glEnable(GL_DEPTH_TEST);
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
    camConfig.perspective.fov = glm::radians(60.0f);
    // camConfig.ortho.size = 3.0f;
    camConfig.perspective.aspectRatio = 1280.0f / 720.0f;
    camConfig.perspective.near = 0.1f;
    camConfig.perspective.far = 100.0f;
    cam = new Camera(
        Camera::Type::PERSPECTIVE,
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
    static glm::mat4 identityMat = glm::identity<glm::mat4>();
    static glm::mat4 translated = glm::translate(identityMat, glm::vec3(0.3f, 0.3f, 1.0f));
    static glm::vec3 position = cam->GetPosition();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Bind();

    Input &input = Application::GetInstance().GetMainWindow()->input;
    if(input.IsKeyPressed(GLFW_KEY_D))
    {
        cam->SetPosition(position += glm::vec3(0.005f, 0.0f, 0.0f));
    }

    if(input.IsKeyPressed(GLFW_KEY_RIGHT))
    {
        cam->SetRotation( 
            glm::rotate(
                cam->GetRotation(), 
                glm::radians(-1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            )
        );
    }
    else if(input.IsKeyPressed(GLFW_KEY_LEFT))
    {
        cam->SetRotation( 
            glm::rotate(
                cam->GetRotation(), 
                glm::radians(1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            )
        );
    }

    shader->UploadMat4(
        "u_viewProjection",
        cam->GetViewProjection()
    );

    shader->UploadMat4(
        "u_model",
        identityMat
    );

    shader->UploadVec3(
        "u_color",
        glm::vec3(0.8f, 0.3f, 0.2f)
    );

    glEnableVertexAttribArray(0);
    vertexBuffer->Bind();
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

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
        glm::vec3(0.3f, 0.8f, 0.2f)
    );
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