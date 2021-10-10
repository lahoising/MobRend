#include <mobrend/vertex_layout.h>

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
    uint32_t currentSize = 0;

    for(auto &attrib : this->attributes)
    {
        currentSize += VertexLayout::GetAttributeSize(attrib);
    }
    this->stride = currentSize;
}

uint32_t VertexLayout::GetAttributeSize(const Attribute &attrib)
{
    switch (attrib.type)
    {
    case AttributeType::ATTRIBUTE_TYPE_FLOAT: return sizeof(float) * attrib.count;
    default: throw "Unknow attribute type provided";
    }
}

void VertexLayout::PushAttribute(const Attribute &attribute)
{
    this->attributes.push_back(attribute);
    this->stride += VertexLayout::GetAttributeSize(attribute);
}

} // namespace mr
