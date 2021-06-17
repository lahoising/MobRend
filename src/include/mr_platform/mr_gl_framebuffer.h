#ifndef _MR_GL_FRAMEBUFFER_H_
#define _MR_GL_FRAMEBUFFER_H_

#include <vector>
#include "mr_framebuffer.h"
#include "mr_texture.h"

namespace mr
{
    
class GlFramebuffer : public Framebuffer
{
public:
    struct GlAttachment
    {
        bool isRenderbufferObject;
        union
        {
            unsigned int renderObjectId;
            Texture *texture;
        };
    };

public:
    GlFramebuffer(Framebuffer::CreateParams &createParams);
    virtual ~GlFramebuffer() override;
    virtual void Bind(FramebufferUsage usage) override;
    virtual void Unbind(FramebufferUsage usage) override;

private:
    static GlAttachment CreateAttachment(const Framebuffer::Attachment &specifications);
    static unsigned int GetFramebufferUsage(mr::FramebufferUsage usage);

private:
    unsigned int framebufferId;
    std::vector<GlAttachment> attachments;
};

} // namespace mr

#endif