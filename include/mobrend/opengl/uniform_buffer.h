#ifndef _MR_GL_UNIFORM_BUFFER_H_
#define _MR_GL_UNIFORM_BUFFER_H_

#include "mr_uniform_buffer.h"

namespace mr
{
    
class GlUniformBuffer : public UniformBuffer
{
public:
    GlUniformBuffer(const UniformBuffer::CreateParams &params);
    virtual ~GlUniformBuffer() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetData(const void *data, uint32_t size, uint32_t offset) override;

    unsigned int GetBufferId(){ return this->bufferId; }
    uint32_t GetBinding(){ return this->binding; }

private:
    unsigned int bufferId;
    uint32_t binding;
};

} // namespace mr

#endif