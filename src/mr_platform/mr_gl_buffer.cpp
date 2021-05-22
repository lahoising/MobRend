#include <unordered_map>
#include "glad/glad.h"
#include "mr_platform/mr_gl_buffer.h"

namespace mr
{

static std::unordered_map<Buffer::Type,GLenum> bufferTypeMap = {
    {Buffer::Type::VERTEX, GL_ARRAY_BUFFER},
    {Buffer::Type::INDEX, GL_ELEMENT_ARRAY_BUFFER},
};
    
GlBuffer::GlBuffer(Buffer::CreateParams params)
{
    this->type = params.type;
    glGenBuffers(1, &this->bufferId);
    GLenum glType = bufferTypeMap[this->type];
    glBindBuffer(glType, this->bufferId);
    glBufferData(glType, params.size, params.data, GL_STATIC_DRAW);

    switch (this->type)
    {
    case Buffer::Type::INDEX:
        this->elementCount = (uint32_t)(params.size / sizeof(uint32_t));
        break;
    
    default:
        break;
    }
}

GlBuffer::~GlBuffer()
{
    glDeleteBuffers(1, &this->bufferId);
}

void GlBuffer::Bind()
{
    GLenum glType = bufferTypeMap[this->type];
    glBindBuffer(glType, this->bufferId);
}

void GlBuffer::Unbind()
{
    GLenum glType = bufferTypeMap[this->type];
    glBindBuffer(glType, 0);
}


} // namespace mr
