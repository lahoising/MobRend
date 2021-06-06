#ifndef _MR_BUFFER_H_
#define _MR_BUFFER_H_

namespace mr
{
    
class Buffer
{
public:
    virtual ~Buffer() = 0 {};
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
};

} // namespace mr

#endif