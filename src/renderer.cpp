#include "mr_renderer.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_renderer.h"
#endif

namespace mr
{
    Renderer *Renderer::Create()
    {
        #ifdef MOBREND_GL_RENDERING
        return new GlRenderer();
        #else
        return nullptr;
        #endif
    }
} // namespace mr
