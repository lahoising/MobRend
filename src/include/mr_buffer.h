#ifndef _MR_BUFFER_H_
#define _MR_BUFFER_H_

#include <inttypes.h>

namespace mr
{
    
class Buffer
{
public:
    enum Type
    {
        VERTEX,
        INDEX
    };

    typedef struct
    {
        void *data;
        size_t size;
        Type type;
    } CreateParams;

public:
    static Buffer *Create(CreateParams params);
    
    virtual ~Buffer() = 0 {};
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    Type GetType(){ return this->type; }
    uint32_t GetElementCount(){ return this->elementCount; }

protected:
    Type type;
    uint32_t elementCount;
};

} // namespace mr


#endif