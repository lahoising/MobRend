#ifndef _MR_GL_RENDERER_H_
#define _MR_GL_RENDERER_H_
#ifdef MOBREND_GL_RENDERING

#include <glad/glad.h>
#include "mr_renderer.h"
#include "mr_vertex_layout.h"

namespace mr
{
    
class GlRenderer : public Renderer
{
public:
    GlRenderer();
    virtual ~GlRenderer() override;
    virtual void Clear() override;
    virtual void SetViewport(int x, int y, int width, int height) override;
    virtual void OnRenderBegin() override;
    virtual void OnRenderEnd() override;
    virtual Renderer::gui_init_info_s *GetGuiInitInfo() override;
    virtual void DeleteGuiInitInfo(Renderer::gui_init_info_s *info) override;
    virtual void Render(Renderer::Command &cmd) override;

    virtual void EnableRenderPass(RenderPassMask renderPassMask, bool enable) override;
    virtual void EnableFeatures(RendererFeatureMask mask, bool enable) override;
    
    virtual void SetDepthTestFn(RenderPassFn fn) override;
    void SetDepthMask(bool writeable) override;
    
    virtual void SetStencilTestFn(RenderPassFn fn, int refValue, unsigned int mask) override;
    virtual void SetStencilTestAction(
        StencilAction stencilFailAction, 
        StencilAction depthFailAction, 
        StencilAction bothPass) override;
    virtual void SetStencilMask(uint32_t mask) override;
    
    virtual void SetBlendFn(BlendFn srcFactor, BlendFn dstFactor) override;

    virtual void SetCullFace(CullFace cullFace) override;
    virtual void SetFrontFaceWinding(FrontFaceWinding winding) override;

private:
    static GLenum GetTopology(TopologyType type);
    static GLenum GetRendererFeature(RendererFeature feature);
    static GLenum GetRenderPass(RenderPass pass);
    static GLenum GetRenderPassFn(RenderPassFn fn);
    static GLenum GetStencilAction(StencilAction action);
    static GLenum GetBlendFn(BlendFn fn);
    static GLenum GetCullFace(CullFace cullFace);
    static GLenum GetWinding(FrontFaceWinding winding);
};

} // namespace mr

#endif
#endif