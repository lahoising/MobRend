#ifndef _MR_RENDERER_H_
#define _MR_RENDERER_H_

namespace mr
{

class Renderer
{
public:
    static Renderer *Create();
    virtual ~Renderer() = 0 {};
    virtual void OnRenderBegin() = 0;
    virtual void OnRenderEnd() = 0;
};
    
} // namespace mr


#endif