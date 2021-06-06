#include "mr_mesh.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_mesh.h"
#endif

namespace mr
{
    
Mesh *Mesh::Create(CreateParams &params)
{
    #ifdef MOBREND_GL_RENDERING
    return new GlMesh(params);
    #else
    return nullptr;
    #endif
}

} // namespace mr