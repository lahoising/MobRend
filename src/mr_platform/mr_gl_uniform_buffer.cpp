#include <glad/glad.h>
#include "mr_platform/mr_gl_uniform_buffer.h"

namespace mr
{
    
GlUniformBuffer::GlUniformBuffer(const UniformBuffer::CreateParams &params)
{
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, this->bufferId);
    glBufferData(GL_UNIFORM_BUFFER, params.bufferSize, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    this->binding = params.binding;
    glBindBufferRange(
        GL_UNIFORM_BUFFER,
        this->binding,
        this->bufferId,
        0, params.bufferSize);
}

void GlUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, this->bufferId);
    glBufferSubData(GL_UNIFORM_BUFFER, (int)offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GlUniformBuffer::~GlUniformBuffer()
{
    glDeleteBuffers(1, &this->bufferId);
}

void GlUniformBuffer::Bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER,this->bufferId);
}
void GlUniformBuffer::Unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER,this->bufferId);
}

} // namespace mr
