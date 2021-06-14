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

#include <assimp/Importer.hpp>

namespace mr
{

struct Renderer::gui_init_info_s
{
    char glVersion[16];
};

GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
}

GlRenderer::~GlRenderer()
{
}

void GlRenderer::SetViewport(int x, int y, int width, int height) 
{
    glViewport(x, y, width, height);
}

void GlRenderer::OnRenderBegin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    case RenderObjectType::RENDER_OBJECT_MESH:
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
    
    case RenderObjectType::RENDER_OBJECT_MODEL:
        cmd.renderObjectType = RenderObjectType::RENDER_OBJECT_MESH;
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

void GlRenderer::EnableRenderPass(RenderPassMask renderPassMask, bool enable)
{
    void (*fn)(GLenum) = enable? glEnable : glDisable;
    if(renderPassMask & RENDER_PASS_DEPTH)
    {
        fn( GlRenderer::GetRenderPass(RENDER_PASS_DEPTH) );
    }
    if(renderPassMask & RENDER_PASS_STENCIL)
    {
        fn( GlRenderer::GetRenderPass(RENDER_PASS_STENCIL) );
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
    case TopologyType::TOPOLOGY_TRIANGLES: return GL_TRIANGLES;
    case TopologyType::TOPOLOGY_WIREFRAME: return GL_LINE_STRIP;
    default: throw "Unsupported topology type";
    }
}

GLenum GlRenderer::GetRenderPass(RenderPass pass)
{
    switch (pass)
    {
    case RENDER_PASS_DEPTH: GL_DEPTH_TEST;
    case RENDER_PASS_STENCIL: GL_STENCIL_TEST;
    default: return 0;
    }
}

} // namespace mr


#endif