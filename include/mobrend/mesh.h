#ifndef _MR_MESH_H_
#define _MR_MESH_H_

#include <unordered_map>
#include <string>
#include <mobrend/buffer.h>
#include <mobrend/vertex_layout.h>
#include <mobrend/vertex_buffer.h>
#include <mobrend/index_buffer.h>
#include <mobrend/texture.h>
#include <mobrend/material.h>

namespace mr
{

class Mesh
{
public:
    typedef struct
    {
        const VertexLayout *vertexLayout;
        const void *vertices;
        uint32_t verticesArraySize;
        const uint32_t *indices;
        uint32_t indexCount;
		Material *mat = nullptr;
    } CreateParams;

public:
    // static Mesh *Create(CreateParams &params);
    Mesh(const CreateParams &params);
    ~Mesh();

    const VertexBuffer *GetVertexBuffer() const;
    const IndexBuffer *GetIndexBuffer() const;

protected:
    VertexBuffer *vertexBuffer;
    IndexBuffer *indexBuffer;
	Material *mat;
};

}

#endif
