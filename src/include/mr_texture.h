#ifndef _MR_TEXTURE_H_
#define _MR_TEXTURE_H_

#include <inttypes.h>

namespace mr
{
    
class Texture
{
public:
    static Texture *Create(const char *filepath);
    virtual ~Texture() = 0 {};
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    uint32_t GetWidth(){ return this->w; }
    uint32_t GetHeight(){ return this->h; }

protected:
    uint32_t w, h;
};

} // namespace mr

#endif