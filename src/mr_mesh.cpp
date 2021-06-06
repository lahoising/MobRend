#include "mr_logger.h"
#include "mr_mesh.h"

// #ifdef MOBREND_GL_RENDERING
// #include "mr_platform/mr_gl_mesh.h"
// #endif

namespace mr
{
    
// Mesh *Mesh::Create(CreateParams &params)
// {
//     #ifdef MOBREND_GL_RENDERING
//     return new GlMesh(params);
//     #else
//     return nullptr;
//     #endif
// }

Mesh::Mesh(CreateParams &params)
    : vertexBuffer(params.vertexBuffer), indexBuffer(params.indexBuffer) 
{}

Mesh::~Mesh()
{
    MR_ASSERT(this->vertexBuffer);
    delete(this->vertexBuffer);
    this->vertexBuffer = nullptr;

    MR_ASSERT(this->indexBuffer);
    delete(this->indexBuffer);
    this->indexBuffer = nullptr;
}

const VertexBuffer *Mesh::GetVertexBuffer() const { return this->vertexBuffer; }
const IndexBuffer *Mesh::GetIndexBuffer() const { return this->indexBuffer; }

} // namespace mr