#ifndef _MR_SHADER_H_
#define _MR_SHADER_H_

#include <glm/glm.hpp>

namespace mr
{
class Texture;
    
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

    /// Remember to bind the shader before uploading any uniform
    virtual void UploadMat3(const char *uniformName,const glm::mat3 &matrix) = 0;
    virtual void UploadMat4(const char *uniformName,const glm::mat4 &matrix) = 0;
    virtual void UploadFloat(const char *uniformName, float val) = 0;
    virtual void UploadVec3(const char *uniformName,const glm::vec3 &vec) = 0;
    virtual void UploadVec4(const char *uniformName,const glm::vec4 &vec) = 0;
    virtual void UploadInt(const char *uniformName, int32_t i) = 0;
    virtual void UploadBool(const char *uniformName, bool val) = 0;
    virtual void UploadTexture(const char *uniformName, Texture *texture) = 0;
};

} // namespace mr

#endif