#include "mr_buffer.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_buffer.h"
#endif

namespace mr
{
    
Buffer *Buffer::Create(CreateParams params)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlBuffer(params);
    #else
    return nullptr;
    #endif
}

} // namespace mr
