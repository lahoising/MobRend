#ifndef _MR_SHADER_H_
#define _MR_SHADER_H_

namespace mr
{
    
class Shader
{
public:
    typedef struct
    {
        const char *vertFilePath;
        const char *fragFilePath;
    } CreateParams;

public:
    static Shader *Create(CreateParams params);
    virtual ~Shader() = 0 {};
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
};

} // namespace mr

#endif