#include "mr_logger.h"
#include <mobrend/mesh.h>

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

Mesh::Mesh(const CreateParams &params)
{
    VertexBuffer::CreateParams vertexCreateParams = {};
    vertexCreateParams.bufferSize = params.verticesArraySize;
    vertexCreateParams.data = params.vertices;
    vertexCreateParams.vertexLayout = params.vertexLayout;
    this->vertexBuffer = VertexBuffer::Create(vertexCreateParams);

    IndexBuffer::CreateParams indexCreateParams = {};
    indexCreateParams.data = params.indices;
    indexCreateParams.elementCount = params.indexCount;
    this->indexBuffer = IndexBuffer::Create(indexCreateParams);

	this->mat = params.mat;
}

Mesh::~Mesh()
{
    MR_ASSERT(this->vertexBuffer);
    delete(this->vertexBuffer);
    this->vertexBuffer = nullptr;

    MR_ASSERT(this->indexBuffer);
    delete(this->indexBuffer);
    this->indexBuffer = nullptr;

	this->mat = nullptr;
}

const VertexBuffer *Mesh::GetVertexBuffer() const { return this->vertexBuffer; }
const IndexBuffer *Mesh::GetIndexBuffer() const { return this->indexBuffer; }

} // namespace mr
