#ifdef MOBREND_GL_RENDERING

#include <glad/glad.h>
#include <glm/glm.hpp>

#ifdef MOBREND_GLFW_WINDOW
#include <mobrend/glfw/window.h>
#endif

#include "mr_logger.h"
#include <mobrend/opengl/renderer.h>
#include <mobrend/gui.h>
#include <mobrend/application.h>

#include <mobrend/model.h>
#include <mobrend/mesh.h>

#include <assimp/Importer.hpp>

namespace mr
{

struct Renderer::gui_init_info_s
{
    char glVersion[16];
};

#ifndef NDEBUG
static void GLAPIENTRY GlErrorMessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    const char *msg = "OpenGL error 0x%x (severity 0x%x): %s";
    if(type == GL_DEBUG_TYPE_ERROR)
    {
        mrerr(msg, type, severity, message);
    }
    else
    {
        mrwarn(msg, type, severity, message);
    }
}
#endif

GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif

    #ifndef NDEBUG
    mrlog("enable gl debug output");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GlErrorMessageCallback, 0);
    #endif

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
}

GlRenderer::~GlRenderer()
{
}

void GlRenderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void GlRenderer::SetViewport(int x, int y, int width, int height) 
{
    glViewport(x, y, width, height);
}

void GlRenderer::OnRenderBegin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GlRenderer::OnRenderEnd()
{
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
    if(renderPassMask & RENDER_PASS_BLEND)
    {
        fn( GlRenderer::GetRenderPass(RENDER_PASS_BLEND) );
    }
    if(renderPassMask & RENDER_PASS_CULLING)
    {
        fn( GlRenderer::GetRenderPass(RENDER_PASS_CULLING) );
    }
}

void GlRenderer::EnableFeatures(RendererFeatureMask mask, bool enable)
{
    void (*fn)(GLenum) = enable? glEnable : glDisable;
    if(mask & RENDERER_FEATURE_SHADER_POINT_SIZE)
    {
        fn( GlRenderer::GetRendererFeature(RENDERER_FEATURE_SHADER_POINT_SIZE) );
    }
}

void GlRenderer::SetDepthTestFn(RenderPassFn fn)
{
    glDepthFunc( GlRenderer::GetRenderPassFn(fn) );
}

void GlRenderer::SetDepthMask(bool writeable)
{
    glDepthMask(writeable? GL_TRUE : GL_FALSE);
}

void GlRenderer::SetStencilTestFn(RenderPassFn fn, int refValue, unsigned int mask)
{
    glStencilFunc(
        GlRenderer::GetRenderPassFn(fn),
        refValue, mask
    );
}

void GlRenderer::SetBlendFn(BlendFn srcFactor, BlendFn dstFactor)
{
    glBlendFunc(
        GlRenderer::GetBlendFn(srcFactor),
        GlRenderer::GetBlendFn(dstFactor)
    );
}

void GlRenderer::SetStencilTestAction(
    StencilAction stencilFailAction, 
    StencilAction depthFailAction, 
    StencilAction bothPass)
{
    glStencilOp(
        GlRenderer::GetStencilAction(stencilFailAction),
        GlRenderer::GetStencilAction(depthFailAction),
        GlRenderer::GetStencilAction(bothPass)
    );
}

void GlRenderer::SetStencilMask(uint32_t mask)
{
    glStencilMask((unsigned int)mask);
}

void GlRenderer::SetCullFace(CullFace cullFace)
{
    glCullFace( GlRenderer::GetCullFace(cullFace) );
}

void GlRenderer::SetFrontFaceWinding(FrontFaceWinding winding)
{
    glFrontFace( GlRenderer::GetWinding(winding) );
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
    case TopologyType::TOPOLOGY_POINTS: return GL_POINTS;
    default: throw "Unsupported topology type";
    }
}

GLenum GlRenderer::GetRendererFeature(RendererFeature feature)
{
    switch (feature)
    {
    case RENDERER_FEATURE_SHADER_POINT_SIZE: return GL_PROGRAM_POINT_SIZE;
    default: return 0;
    }
}

GLenum GlRenderer::GetRenderPass(RenderPass pass)
{
    switch (pass)
    {
    case RENDER_PASS_DEPTH: return GL_DEPTH_TEST;
    case RENDER_PASS_STENCIL: return GL_STENCIL_TEST;
    case RENDER_PASS_BLEND: return GL_BLEND;
    case RENDER_PASS_CULLING: return GL_CULL_FACE;
    default: return 0;
    }
}

GLenum GlRenderer::GetRenderPassFn(RenderPassFn fn)
{
    switch (fn)
    {
    case RENDER_PASS_FN_ALWAYS: return GL_ALWAYS;
    case RENDER_PASS_FN_NEVER: return GL_NEVER;
    case RENDER_PASS_FN_LESS: return GL_LESS;
    case RENDER_PASS_FN_EQUAL: return GL_EQUAL;
    case RENDER_PASS_FN_LESS_OR_EQUEAL: return GL_LEQUAL;
    case RENDER_PASS_FN_GREATER: return GL_GREATER;
    case RENDER_PASS_FN_NOT_EQUAL: return GL_NOTEQUAL;
    case RENDER_PASS_FN_GREATER_OR_EQUAL: return GL_GEQUAL;
    default: throw "Render pass function not supporter";
    }
}

GLenum GlRenderer::GetStencilAction(StencilAction action)
{
    switch (action)
    {
    case STENCIL_ACTION_KEEP: return GL_KEEP;
    case STENCIL_ACTION_ZERO: return GL_ZERO;
    case STENCIL_ACTION_REPLACE: return GL_REPLACE;
    case STENCIL_ACTION_INCREASE: return GL_INCR;
    case STENCIL_ACTION_INCREASE_WRAP: return GL_INCR_WRAP;
    case STENCIL_ACTION_DECREASE: return GL_DECR;
    case STENCIL_ACTION_DECREASE_WRAP: return GL_DECR_WRAP;
    case STENCIL_ACTION_INVERT: return GL_INVERT;
    default: throw "Unknown stencil action";
    }
}

GLenum GlRenderer::GetBlendFn(BlendFn fn)
{
    switch(fn)
    {
    case BLEND_FN_ZERO:                     return GL_ZERO;
    case BLEND_FN_ONE:                      return GL_ONE;
    case BLEND_FN_SRC_COLOR:                return GL_SRC_COLOR;
    case BLEND_FN_ONE_MINUS_SRC_COLOR:      return GL_ONE_MINUS_SRC_COLOR;
    case BLEND_FN_DST_COLOR:                return GL_DST_COLOR;
    case BLEND_FN_ONE_MINUS_DST_COLOR:      return GL_ONE_MINUS_DST_COLOR;
    case BLEND_FN_SRC_ALPHA:                return GL_SRC_ALPHA;
    case BLEND_FN_ONE_MINUS_SRC_ALPHA:      return GL_ONE_MINUS_SRC_ALPHA;
    case BLEND_FN_DST_ALPHA:                return GL_DST_ALPHA;
    case BLEND_FN_ONE_MINUS_DST_ALPHA:      return GL_ONE_MINUS_DST_ALPHA;
    case BLEND_FN_CONSTANT_COLOR:           return GL_CONSTANT_COLOR;
    case BLEND_FN_ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
    case BLEND_FN_CONSTANT_ALPHA:           return GL_CONSTANT_ALPHA;
    case BLEND_FN_ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
    default: throw "Unknown blend fn";
    }
}

GLenum GlRenderer::GetCullFace(CullFace cullFace)
{
    switch (cullFace)
    {
    case CULL_FACE_FRONT:           return GL_FRONT;
    case CULL_FACE_BACK:            return GL_BACK;
    case CULL_FACE_FRONT_AND_BACK:  return GL_FRONT_AND_BACK;
    default: throw "Unknown cull face";
    }
}

GLenum GlRenderer::GetWinding(FrontFaceWinding winding)
{
    switch (winding)
    {
    case FF_WINDING_CCW:    return GL_CCW;
    case FF_WINDING_CW:     return GL_CW;
    default: throw "Unknown winding";
    }
}


} // namespace mr


#endif
