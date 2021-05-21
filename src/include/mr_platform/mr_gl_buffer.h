#ifndef _MR_GL_BUFFER_H_
#define _MR_GL_BUFFER_H_

#include "mr_buffer.h"

namespace mr
{
    
class GlBuffer : public Buffer
{
public:
    GlBuffer(Buffer::CreateParams params);
    virtual ~GlBuffer() override;

    virtual void Bind() override;
    virtual void Unbind() override;

    uint32_t GetId(){ return this->bufferId; }

private:
    uint32_t bufferId;

    union
    {
        uint32_t indexCount;    // Only used in index buffers
        uint32_t vertexArrayId; // Only used in vertex buffers
    };
};

} // namespace mr

#endif