#ifndef _MR_FRAMEBUFFER_H_
#define _MR_FRAMEBUFFER_H_

#include <vector>
#include "mr_texture.h"

namespace mr
{

enum FramebufferUsage
{
    FRAMEBUFFER_USAGE_READ_WRITE,
    FRAMEBUFFER_USAGE_READ,
    FRAMEBUFFER_USAGE_WRITE
};

class Framebuffer
{
public:
    enum AttachmentType
    {
        ATTACHMENT_COLOR_0,
        ATTACHMENT_DEPTH,
        ATTACHMENT_DEPTH24_STENCIL8,
    };

    struct Attachment
    {
        AttachmentType type;
        bool isRenderbufferObject;
    };

    struct CreateParams
    {
        std::vector<Attachment> attachments;
        uint32_t width, height;
    };

public:
    static Framebuffer *Create(CreateParams &createParams);
    virtual ~Framebuffer() = 0 {};
    virtual void Bind(FramebufferUsage usage) = 0;
    virtual void Unbind(FramebufferUsage usage) = 0;
    virtual void Clear(FramebufferUsage usage) = 0;
    virtual Texture *GetTextureAttachment(uint32_t index) = 0;
};

} // namespace mr

#endif