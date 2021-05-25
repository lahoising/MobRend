#include "mr_vertex_layout.h"

namespace mr
{
    
VertexLayout::VertexLayout(const std::vector<Attribute> &attributes)
{
    this->attributes = attributes;
    this->CalculateStride();
}

VertexLayout::VertexLayout(const std::initializer_list<Attribute> attributes)
    : VertexLayout( std::vector<Attribute>(attributes) ) {}

void VertexLayout::CalculateStride()
{
    size_t currentSize = 0;

    for(auto &attrib : this->attributes)
    {
        currentSize += VertexLayout::GetAttributeSize(attrib);
    }
    this->stride = currentSize;
}

size_t VertexLayout::GetAttributeSize(const Attribute &attrib)
{
    switch (attrib.type)
    {
    case AttributeType::FLOAT: return sizeof(float) * attrib.count;
    default: throw "Unknow attribute type provided";
    }
}

} // namespace mr
