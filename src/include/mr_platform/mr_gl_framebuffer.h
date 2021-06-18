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
        union
        {
            unsigned int renderObjectId;
            GlTexture *texture;
        };
        Framebuffer::AttachmentType type;
        bool isRenderbufferObject;
    };

public:
    GlFramebuffer(Framebuffer::CreateParams &createParams);
    virtual ~GlFramebuffer() override;
    virtual void Bind(FramebufferUsage usage) override;
    virtual void Unbind(FramebufferUsage usage) override;
    virtual void Clear(FramebufferUsage usage) override;
    virtual Texture *GetTextureAttachment(uint32_t index) override;

private:
    GlAttachment CreateAttachment(const Framebuffer::Attachment &specifications);

private:
    static unsigned int GetFramebufferUsage(mr::FramebufferUsage usage);
    static unsigned int GetAttachment(mr::Framebuffer::AttachmentType attachmentType);
    static unsigned int GetAttachmentInternalFormat(Framebuffer::AttachmentType attachmentType);
    static unsigned int GetBufferBit(Framebuffer::AttachmentType type);

private:
    unsigned int framebufferId;
    uint32_t width, height;
    std::vector<GlAttachment> attachments;
};

} // namespace mr

#endif