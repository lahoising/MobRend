#ifndef _MR_GL_FRAMEBUFFER_H_
#define _MR_GL_FRAMEBUFFER_H_

#include <vector>
#include "mr_framebuffer.h"
#include "mr_platform/mr_gl_texture.h"

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
            GlTexture *texture;
        };
    };

public:
    GlFramebuffer(Framebuffer::CreateParams &createParams);
    virtual ~GlFramebuffer() override;
    virtual void Bind(FramebufferUsage usage) override;
    virtual void Unbind(FramebufferUsage usage) override;

private:
    GlAttachment CreateAttachment(const Framebuffer::Attachment &specifications);

private:
    static unsigned int GetFramebufferUsage(mr::FramebufferUsage usage);
    static unsigned int GetFramebufferAttachment(mr::Framebuffer::AttachmentType attachmentType);

private:
    unsigned int framebufferId;
    uint32_t width, height;
    std::vector<GlAttachment> attachments;
};

} // namespace mr

#endif