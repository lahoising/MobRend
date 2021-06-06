#ifndef _MR_INDEX_BUFFER_H_
#define _MR_INDEX_BUFFER_H_

#include <inttypes.h>
#include "mr_buffer.h"

namespace mr
{
    
class IndexBuffer : public Buffer
{
public:
    typedef struct
    {
        const uint32_t *data;
        uint32_t elementCount;
    } CreateParams;

public:
    static IndexBuffer *Create(CreateParams &params);
    virtual ~IndexBuffer() = 0 {};
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    uint32_t GetElementCount() { return this->elementCount; }

protected:
    uint32_t elementCount;
};

} // namespace mr

#endif