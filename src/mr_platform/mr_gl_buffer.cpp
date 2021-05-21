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
    GLenum glType = bufferTypeMap[this->type];

    switch (this->type)
    {
    case Buffer::Type::VERTEX:
        glGenVertexArrays(1, &this->vertexArrayId);
        glBindVertexArray(this->vertexArrayId);
        break;

    case Buffer::Type::INDEX:
        this->indexCount = params.size / sizeof(uint32_t);
        break;
    
    default:
        break;
    }

    glGenBuffers(1, &this->bufferId);
    
    glBindBuffer(glType, this->bufferId);
    glBufferData(glType, params.size, params.data, GL_STATIC_DRAW);
    this->Unbind();
}

GlBuffer::~GlBuffer()
{
    glDeleteBuffers(1, &this->bufferId);
    if(this->type == Buffer::Type::VERTEX)
        glDeleteVertexArrays(1, &this->vertexArrayId);
}

void GlBuffer::Bind()
{
    GLenum glType = bufferTypeMap[this->type];
    glBindBuffer(glType, this->bufferId);

    if(this->type == Buffer::Type::INDEX)
    {
        glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, nullptr);
    }
}

void GlBuffer::Unbind()
{
    GLenum glType = bufferTypeMap[this->type];
    glBindBuffer(glType, 0);

    if(this->type == Buffer::Type::VERTEX)
    {
        glBindVertexArray(0);
    }
}


} // namespace mr
