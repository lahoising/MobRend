#include <mobrend/index_buffer.h>

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_index_buffer.h"
#endif

namespace mr
{
    
IndexBuffer *IndexBuffer::Create(CreateParams &params)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlIndexBuffer(params);
    #else
    return nullptr;
    #endif
}

} // namespace mr
