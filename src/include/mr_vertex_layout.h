#ifndef _MR_VERTEX_LAYOUT_H_
#define _MR_VERTEX_LAYOUT_H_

#include <inttypes.h>
#include <vector>
// #include <initializer_list>

namespace mr
{

enum AttributeType
{
    FLOAT
};

typedef struct
{
    AttributeType type;
    uint32_t count;
} Attribute;
    
class VertexLayout
{
public:
    VertexLayout(const std::vector<Attribute> &attributes);
    VertexLayout(const std::initializer_list<Attribute> attributes);
    uint64_t GetStride(){ return this->stride; }
    const std::vector<Attribute> &GetAttributes(){ return this->attributes; }

private:
    void CalculateStride();

public:
    static uint64_t GetAttributeSize(const Attribute &attrib);

private:
    uint64_t stride;
    std::vector<Attribute> attributes;
};

} // namespace mr

#endif