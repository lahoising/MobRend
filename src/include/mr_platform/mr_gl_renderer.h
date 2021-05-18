#ifndef _MR_GL_RENDERER_H_
#define _MR_GL_RENDERER_H_

#include "mr_renderer.h"

namespace mr
{
    
class GlRenderer : public Renderer
{
public:
    GlRenderer();
    virtual ~GlRenderer() override;
};

} // namespace mr

#endif