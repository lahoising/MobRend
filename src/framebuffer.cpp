#include <mobrend/framebuffer.h>

#ifdef MOBREND_GL_RENDERING
#include <mobrend/opengl/framebuffer.h>
#endif

namespace mr
{
    
Framebuffer *Framebuffer::Create(CreateParams &createParams)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlFramebuffer(createParams);
    #else
    return nullptr;
    #endif
}

} // namespace mr
