#include <glad/glad.h>
#include "mr_platform/mr_gl_mesh.h"

namespace mr
{
    
GlMesh::GlMesh(Mesh::CreateParams &params)
{
    this->vertexBuffer = params.vertexBuffer;
    this->indexBuffer = params.indexBuffer;
    this->textures = params.textures;
}

GlMesh::~GlMesh()
{
    delete(this->vertexBuffer);
    delete(this->indexBuffer);
    for(auto &pair : this->textures)
    {
        delete(pair.second);
    }
    this->textures.clear();
}

} // namespace mr