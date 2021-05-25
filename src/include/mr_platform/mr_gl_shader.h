#ifndef _MR_GL_SHADER_H_
#define _MR_GL_SHADER_H_

#include <unordered_map>
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

    virtual void UploadMat4(const char *uniformName, glm::mat4 matrix) override;
    virtual void UploadVec3(const char *uniformName, glm::vec3 vec) override;
    virtual void UploadInt(const char *uniformName, int i) override;
    virtual void UploadTexture2D(const char *uniformName, Texture *texture) override;

private:
    void CompileShader(unsigned int shaderId, const char *shaderSource);

private:
    unsigned int programId;
    std::unordered_map<std::string,unsigned int> uniformLocations;
};

} // namespace mr

#endif