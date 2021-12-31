#ifndef _MR_GL_INDEX_BUFFER_H_
#define _MR_GL_INDEX_BUFFER_H_

#include <mobrend/index_buffer.h>

namespace mr
{
    
class GlIndexBuffer : public IndexBuffer
{
public:
    GlIndexBuffer(IndexBuffer::CreateParams &params);
    virtual ~GlIndexBuffer() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    unsigned int bufferId;
};

} // namespace mr

#endif
