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

    virtual void UploadMat3(const char *uniformName,const glm::mat3 &matrix) override;
    virtual void UploadMat4(const char *uniformName,const glm::mat4 &matrix) override;
    virtual void UploadFloat(const char *uniformName, float val) override;
    virtual void UploadVec3(const char *uniformName,const glm::vec3 &vec) override;
    virtual void UploadInt(const char *uniformName, int i) override;
    virtual void UploadBool(const char *uniformName, bool val) override;
    virtual void UploadTexture2D(const char *uniformName, Texture *texture) override;

private:
    void CompileShader(unsigned int shaderId, const char *shaderSource);

private:
    unsigned int programId;
    std::unordered_map<std::string,unsigned int> uniformLocations;
};

} // namespace mr

#endif