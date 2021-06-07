#ifndef _MR_UNIFORM_LAYOUT_H_
#define _MR_UNIFORM_LAYOUT_H_

#include <unordered_map>

namespace mr {

enum UniformType;

typedef struct
{
    UniformType type;
    unsigned int count;
} UniformValue;

typedef std::unordered_map<std::string,UniformValue> UniformLayout;
UniformLayout GetUniformLayout(const char *source);

enum UniformType
{
    BOOL,
    BOOL2,
    BOOL3,
    BOOL4,
    INT,
    INT2,
    INT3,
    INT4,
    UINT,
    UINT2,
    UINT3,
    UINT4,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    DOUBLE,
    DOUBLE2,
    DOUBLE3,
    DOUBLE4,
    MAT2X2,
    MAT2X3,
    MAT2X4,
    MAT3X2,
    MAT3X3,
    MAT3X4,
    MAT4X2,
    MAT4X3,
    MAT4X4,
    TEX1D,
    TEX2D,
    TEX3D,
};

} // namespace mr

#endif