#ifndef _MR_GL_VERTEX_BUFFER_H_
#define _MR_GL_VERTEX_BUFFER_H_

#include "mr_vertex_buffer.h"
#include "mr_vertex_layout.h"

namespace mr
{
    
class GlVertexBuffer : public VertexBuffer
{
public:
    GlVertexBuffer(VertexBuffer::CreateParams &params);
    virtual ~GlVertexBuffer() override;
    virtual void Bind() override;
    virtual void Unbind() override;

private:
    static unsigned int GetAttribType(AttributeType attributeType);

private:
    unsigned int vao;
    unsigned int bufferId;
};

} // namespace mr

#endif