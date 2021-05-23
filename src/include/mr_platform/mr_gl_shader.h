#ifndef _MR_GL_SHADER_H_
#define _MR_GL_SHADER_H_

#include "mr_shader.h"

namespace mr
{

class GlShader : public Shader
{
public:
    GlShader(Shader::CreateParams params);
    virtual ~GlShader() override;
    virtual void Bind() override;
    virtual void Unbind() override;

private:
    void CompileShader(unsigned int shaderId, const char *shaderSource);

private:
    unsigned int programId;
};

} // namespace mr

#endif