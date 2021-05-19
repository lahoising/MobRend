#ifndef _MR_GL_RENDERER_H_
#define _MR_GL_RENDERER_H_
#ifdef MOBREND_GL_RENDERING

#include "mr_renderer.h"

namespace mr
{
    
class GlRenderer : public Renderer
{
public:
    GlRenderer();
    virtual ~GlRenderer() override;
    virtual void OnRenderBegin() override;
    virtual void OnRenderEnd() override;
};

} // namespace mr

#endif
#endif