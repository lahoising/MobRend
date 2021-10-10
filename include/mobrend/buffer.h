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

#endif
