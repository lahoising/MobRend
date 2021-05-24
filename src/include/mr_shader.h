#ifndef _MR_SHADER_H_
#define _MR_SHADER_H_

#include <glm/glm.hpp>

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

    /// Remember to bind the shader before uploading any uniform
    virtual void UploadMat4(const char *uniformName, glm::mat4 matrix) = 0;
};

} // namespace mr

#endif