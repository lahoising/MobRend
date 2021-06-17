#include <glad/glad.h>
#include "mr_logger.h"
#include "mr_platform/mr_gl_framebuffer.h"

namespace mr
{
    
GlFramebuffer::GlFramebuffer(Framebuffer::CreateParams &createParams)
{
    glGenFramebuffers(1, &this->framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferId);



    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        mrerr("Error creating framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GlFramebuffer::~GlFramebuffer()
{
    glDeleteFramebuffers(1, &this->framebufferId);
}

void GlFramebuffer::Bind(FramebufferUsage usage)
{
    glBindFramebuffer( GlFramebuffer::GetFramebufferUsage(usage), this->framebufferId );
}

void GlFramebuffer::Unbind(FramebufferUsage usage)
{
    glBindFramebuffer( GlFramebuffer::GetFramebufferUsage(usage), 0 );
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

} // namespace mr