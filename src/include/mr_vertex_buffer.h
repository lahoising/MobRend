#ifndef _MR_VERTEX_BUFFER_H_
#define _MR_VERTEX_BUFFER_H_

#include "mr_buffer.h"
#include "mr_vertex_layout.h"

namespace mr
{
    
class VertexBuffer : public Buffer
{
public:
    typedef struct
    {
        const void *data;
        size_t bufferSize;
        const VertexLayout *vertexLayout;
    } CreateParams;

public:
    static VertexBuffer *Create(CreateParams &params);

    virtual ~VertexBuffer() = 0 {};
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};

} // namespace mr

#endif