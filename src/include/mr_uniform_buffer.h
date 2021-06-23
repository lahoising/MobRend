#ifndef _MR_UNIFORM_BUFFER_H_
#define _MR_UNIFORM_BUFFER_H_

#include <inttypes.h>
#include "mr_buffer.h"

namespace mr
{
    
class UniformBuffer : public Buffer
{
public:
    struct CreateParams
    {
        uint32_t bufferSize;
        uint32_t binding;
    };

public:
    static UniformBuffer *Create(const CreateParams &params);
    virtual ~UniformBuffer() = 0 {}
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(void *data, uint32_t size, uint32_t offset) = 0;
};

} // namespace mr

#endif