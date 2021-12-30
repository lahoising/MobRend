#include <mobrend/renderer.h>

#ifdef MOBREND_GL_RENDERING
#include <mobrend/opengl/renderer.h>
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
