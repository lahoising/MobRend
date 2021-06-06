#ifndef _MR_GL_INDEX_BUFFER_H_
#define _MR_GL_INDEX_BUFFER_H_

#include "mr_index_buffer.h"

namespace mr
{
    
class GlIndexBuffer : public IndexBuffer
{
public:
    GlIndexBuffer(IndexBuffer::CreateParams &params);
    virtual ~GlIndexBuffer() override;
    virtual void Bind() override;
    virtual void Unbind() override;

private:
    unsigned int bufferId;
};

} // namespace mr

#endif