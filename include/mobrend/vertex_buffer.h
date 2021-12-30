#ifndef _MR_VERTEX_BUFFER_H_
#define _MR_VERTEX_BUFFER_H_

#include <cinttypes>
#include <mobrend/buffer.h>
#include <mobrend/vertex_layout.h>

namespace mr
{
    
class VertexBuffer : public Buffer
{
public:
    typedef struct
    {
        const void *data;
		std::size_t bufferSize;
        const VertexLayout *vertexLayout;
    } CreateParams;

public:
    static VertexBuffer *Create(CreateParams &params);

    virtual ~VertexBuffer() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};


} // namespace mr

#endif
