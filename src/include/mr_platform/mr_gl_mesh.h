#ifndef _MR_GL_MESH_H_
#define _MR_GL_MESH_H_

#include "mr_mesh.h"

namespace mr
{
    
class GlMesh : public Mesh
{
public:
    GlMesh(Mesh::CreateParams &params);
    virtual ~GlMesh() override;

private:
    unsigned int vao;
};

} // namespace mr

#endif