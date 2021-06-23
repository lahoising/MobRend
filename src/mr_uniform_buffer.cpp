#include "mr_uniform_buffer.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_uniform_buffer.h"
#endif

namespace mr
{
    
UniformBuffer *UniformBuffer::Create(const CreateParams &params)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlUniformBuffer(params);
    #else
    return nullptr;
    #endif
}

} // namespace mr
