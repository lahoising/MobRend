#include <glad/glad.h>
#include "mr_logger.h"
#include "mr_platform/mr_gl_framebuffer.h"

namespace mr
{
    
GlFramebuffer::GlFramebuffer(Framebuffer::CreateParams &createParams)
{
    glGenFramebuffers(1, &this->framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferId);

    this->width = createParams.width;
    this->height = createParams.height;

    for(auto &att : createParams.attachments)
    {
        this->attachments.push_back( GlFramebuffer::CreateAttachment(att) );
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        mrerr("Error creating framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GlFramebuffer::~GlFramebuffer()
{
    glDeleteFramebuffers(1, &this->framebufferId);
    for(auto &att : this->attachments)
    {
        if(att.isRenderbufferObject)
        {
            glDeleteRenderbuffers(1, &att.renderObjectId);
        }
    }
}

GlFramebuffer::GlAttachment GlFramebuffer::CreateAttachment(const Framebuffer::Attachment &att)
{
    GlFramebuffer::GlAttachment ret = {};
    ret.isRenderbufferObject = att.isRenderbufferObject;

    if(ret.isRenderbufferObject)
    {
        glGenRenderbuffers(1, &ret.renderObjectId);
        glBindRenderbuffer(GL_RENDERBUFFER, ret.renderObjectId);
        glRenderbufferStorage(
            GL_RENDERBUFFER, 
            GlFramebuffer::GetAttachmentInternalFormat(att.type), 
            this->width, this->height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GlFramebuffer::GetAttachment(att.type),
            GL_RENDERBUFFER, ret.renderObjectId);
    }
    else
    {
        Texture::CreateParams texCreateParams = {};
        texCreateParams.info.width = this->width;
        texCreateParams.info.height = this->height;
        texCreateParams.content = nullptr;
        const size_t ATTACHMENT_NAME_SIZE = 64;
        char buff[ATTACHMENT_NAME_SIZE] = {};
        snprintf(buff, ATTACHMENT_NAME_SIZE, "framebuffer_attachment_%u_%u_%u", this->framebufferId, this->width, this->height);
        texCreateParams.referenceName = buff;
    
        ret.texture = (GlTexture*)Texture::Create(texCreateParams);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, 
            GlFramebuffer::GetAttachment(att.type),
            GL_TEXTURE_2D,
            ret.texture->GetTextureId(),
            0);
    }

    return ret;
}

void GlFramebuffer::Bind(FramebufferUsage usage)
{
    glBindFramebuffer( GlFramebuffer::GetFramebufferUsage(usage), this->framebufferId );
}

void GlFramebuffer::Unbind(FramebufferUsage usage)
{
    glBindFramebuffer( GlFramebuffer::GetFramebufferUsage(usage), 0 );
}

void GlFramebuffer::Clear(FramebufferUsage usage)
{
    glBindFramebuffer( GlFramebuffer::GetFramebufferUsage(usage), this->framebufferId );
    
    GLenum bufferBitMask = 0;
    for(auto &att : this->attachments)
    {
        bufferBitMask |= bufferBitMask;
    }

    glClear(bufferBitMask);
}

GLenum GlFramebuffer::GetFramebufferUsage(FramebufferUsage usage)
{
    switch (usage)
    {
    case FRAMEBUFFER_USAGE_READ_WRITE:  return GL_FRAMEBUFFER;
    case FRAMEBUFFER_USAGE_READ:        return GL_READ_FRAMEBUFFER;
    case FRAMEBUFFER_USAGE_WRITE:       return GL_DRAW_FRAMEBUFFER;
    default: throw "Unknown framebuffer usage";
    }
}

unsigned int GlFramebuffer::GetAttachment(mr::Framebuffer::AttachmentType attachmentType)
{
    switch (attachmentType)
    {
    case ATTACHMENT_COLOR_0:            return GL_COLOR_ATTACHMENT0;
    case ATTACHMENT_DEPTH:              return GL_DEPTH_ATTACHMENT;
    case ATTACHMENT_DEPTH24_STENCIL8:   return GL_DEPTH_STENCIL_ATTACHMENT;
    default: throw "Unknown attachment type";
    }
}

unsigned int GlFramebuffer::GetAttachmentInternalFormat(Framebuffer::AttachmentType attachmentType)
{
    switch (attachmentType)
    {
    case ATTACHMENT_COLOR_0:            return GL_RGBA;
    case ATTACHMENT_DEPTH:              return GL_DEPTH_COMPONENT32;
    case ATTACHMENT_DEPTH24_STENCIL8:   return GL_DEPTH24_STENCIL8;
    default: throw "Unknown attachment type";
    }
}

unsigned int GlFramebuffer::GetBufferBit(Framebuffer::AttachmentType type)
{
    switch (type)
    {
    case ATTACHMENT_COLOR_0:            return GL_COLOR_BUFFER_BIT;
    case ATTACHMENT_DEPTH:              return GL_DEPTH_BUFFER_BIT;
    case ATTACHMENT_DEPTH24_STENCIL8:   return GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
    default: throw "Unknown framebuffer attachment type";
    }
}

} // namespace mr