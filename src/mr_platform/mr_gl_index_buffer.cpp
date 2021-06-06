#include <glad/glad.h>
#include "mr_platform/mr_gl_index_buffer.h"

namespace mr
{
    
GlIndexBuffer::GlIndexBuffer(IndexBuffer::CreateParams &params)
{
    glGenBuffers(1, &this->bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferId);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        params.elementCount * sizeof(uint32_t),
        (void*)params.data,
        GL_STATIC_DRAW);

    this->elementCount = params.elementCount;
}

GlIndexBuffer::~GlIndexBuffer()
{
    glDeleteBuffers(1, &this->bufferId);
    this->bufferId = 0;
}

void GlIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferId);
}

void GlIndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


} // namespace mr
