#ifndef _MR_UNIFORM_LAYOUT_H_
#define _MR_UNIFORM_LAYOUT_H_

#include <unordered_map>

namespace mr {

enum UniformType
{
    UNIFORM_TYPE_BOOL,
    UNIFORM_TYPE_BOOL2,
    UNIFORM_TYPE_BOOL3,
    UNIFORM_TYPE_BOOL4,
    UNIFORM_TYPE_INT,
    UNIFORM_TYPE_INT2,
    UNIFORM_TYPE_INT3,
    UNIFORM_TYPE_INT4,
    UNIFORM_TYPE_UINT,
    UNIFORM_TYPE_UINT2,
    UNIFORM_TYPE_UINT3,
    UNIFORM_TYPE_UINT4,
    UNIFORM_TYPE_FLOAT,
    UNIFORM_TYPE_VEC2,
    UNIFORM_TYPE_VEC3,
    UNIFORM_TYPE_VEC4,
    UNIFORM_TYPE_DOUBLE,
    UNIFORM_TYPE_DOUBLE2,
    UNIFORM_TYPE_DOUBLE3,
    UNIFORM_TYPE_DOUBLE4,
    UNIFORM_TYPE_MAT2X2,
    UNIFORM_TYPE_MAT2X3,
    UNIFORM_TYPE_MAT2X4,
    UNIFORM_TYPE_MAT3X2,
    UNIFORM_TYPE_MAT3X3,
    UNIFORM_TYPE_MAT3X4,
    UNIFORM_TYPE_MAT4X2,
    UNIFORM_TYPE_MAT4X3,
    UNIFORM_TYPE_MAT4X4,
    UNIFORM_TYPE_TEX1D,
    UNIFORM_TYPE_TEX2D,
    UNIFORM_TYPE_TEX3D,
    UNIFORM_TYPE_UDSTRUCT, // user defined struct
};

typedef struct
{
    UniformType type;
    unsigned int count;
} UniformValue;

typedef std::unordered_map<std::string,UniformValue> UniformLayout;
UniformLayout GetUniformLayout(const char *source);

} // namespace mr

#endif