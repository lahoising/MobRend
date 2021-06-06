#include "mr_vertex_buffer.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_vertex_buffer.h"
#endif

namespace mr
{
    
VertexBuffer *VertexBuffer::Create(CreateParams &params)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlVertexBuffer(params);
    #else
    return nullptr;
    #endif
}

} // namespace mr
