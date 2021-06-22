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
    virtual void UploadVec4(const char *uniformName,const glm::vec4 &vec) override;
    virtual void UploadInt(const char *uniformName, int32_t i) override;
    virtual void UploadBool(const char *uniformName, bool val) override;
    virtual void UploadTexture(const char *uniformName, Texture *texture) override;

private:
    std::string CompileFromSpirV(const std::vector<uint32_t> &source);
    void CompileShader(unsigned int shaderId, const char *shaderSource);

private:
    unsigned int programId;
    unsigned int textureSlotsCount;
    std::unordered_map<unsigned int, unsigned int> textureLocationToTextureSlot;
    std::unordered_map<std::string,unsigned int> uniformLocations;
};

} // namespace mr

#endif