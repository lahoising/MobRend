#ifndef _MR_INDEX_BUFFER_H_
#define _MR_INDEX_BUFFER_H_

#include <inttypes.h>
#include <mobrend/buffer.h>

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
    virtual ~IndexBuffer() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    uint32_t GetElementCount() const { return this->elementCount; }

protected:
    uint32_t elementCount;
};

} // namespace mr

#endif
