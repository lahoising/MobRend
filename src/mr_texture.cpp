#include "mr_texture.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_texture.h"
#endif

namespace mr
{
    
Texture *Texture::Create(const char *filepath)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlTexture(filepath);
    #else
    return nullptr;
    #endif
}

} // namespace mr
