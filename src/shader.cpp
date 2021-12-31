#include <mobrend/shader.h>

#ifdef MOBREND_GL_RENDERING
#include <mobrend/opengl/shader.h>
#endif

namespace mr
{
    
Shader *Shader::Create(CreateParams params)
{
    #if MOBREND_GL_RENDERING
    return new GlShader(params);
    #else
    return nullptr;
    #endif
}

} // namespace mr
