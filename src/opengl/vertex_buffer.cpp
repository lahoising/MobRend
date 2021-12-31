#include <glad/glad.h>
#include <cstdint>
#include <mobrend/opengl/vertex_buffer.h>

namespace mr
{
    
GlVertexBuffer::GlVertexBuffer(VertexBuffer::CreateParams &params)
{
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
    glBufferData(GL_ARRAY_BUFFER, params.bufferSize, params.data, GL_STATIC_DRAW);

    uint32_t offset = 0;
    const std::vector<Attribute> &attributes = params.vertexLayout->GetAttributes();
    for(int i = 0; i < attributes.size(); i++)
    {
        auto attrib = attributes[i];
        uint32_t attribSize = VertexLayout::GetAttributeSize(attrib);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i, 
            attrib.count,
            GetAttribType(attrib.type),
            GL_FALSE,
            params.vertexLayout->GetStride(),
            (const void *)((std::size_t)offset)
        );
        offset += attribSize;
    }
}

GlVertexBuffer::~GlVertexBuffer()
{
    glDeleteBuffers(1, &this->bufferId);
    glDeleteVertexArrays(1, &this->vao);
}

void GlVertexBuffer::Bind() const
{
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
}

void GlVertexBuffer::Unbind() const
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int GlVertexBuffer::GetAttribType(AttributeType attributeType)
{
    switch (attributeType)
    {
    case AttributeType::ATTRIBUTE_TYPE_FLOAT: return GL_FLOAT;
    default: throw "Unsupported attribute type";
    }
}

} // namespace mr
