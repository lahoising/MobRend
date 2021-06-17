#ifndef _MR_GL_FRAMEBUFFER_H_
#define _MR_GL_FRAMEBUFFER_H_

#include "mr_framebuffer.h"

namespace mr
{
    
class GlFramebuffer : public Framebuffer
{
public:
    GlFramebuffer(Framebuffer::CreateParams &createParams);
    virtual ~GlFramebuffer() override;
    virtual void Bind(FramebufferUsage usage) override;
    virtual void Unbind(FramebufferUsage usage) override;

private:
    static unsigned int GetFramebufferUsage(mr::FramebufferUsage usage);

private:
    unsigned int framebufferId;
};

} // namespace mr

#endif