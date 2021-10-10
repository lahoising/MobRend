#ifndef _MR_GL_VERTEX_BUFFER_H_
#define _MR_GL_VERTEX_BUFFER_H_

#include <mobrend/vertex_buffer.h>
#include <mobrend/vertex_layout.h>

namespace mr
{
    
class GlVertexBuffer : public VertexBuffer
{
public:
    GlVertexBuffer(VertexBuffer::CreateParams &params);
    virtual ~GlVertexBuffer() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    static unsigned int GetAttribType(AttributeType attributeType);

private:
    unsigned int vao;
    unsigned int bufferId;
};

} // namespace mr

#endif
