#ifndef _MR_BUFFER_H_
#define _MR_BUFFER_H_

namespace mr
{
    
class Buffer
{
public:
    virtual ~Buffer() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};

} // namespace mr

#include "mr_vertex_buffer.h"
#include "mr_index_buffer.h"
#include "mr_uniform_buffer.h"

#endif