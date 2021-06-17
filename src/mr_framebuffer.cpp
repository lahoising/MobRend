#include "mr_framebuffer.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_framebuffer.h"
#endif

namespace mr
{
    
Framebuffer *Framebuffer::Create(CreateParams &createParams)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlFramebuffer(createParams);
    #else
    return nullptr
    #endif
}

} // namespace mr
