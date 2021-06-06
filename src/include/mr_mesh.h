#ifndef _MR_MESH_H_
#define _MR_MESH_H_

#include <unordered_map>
#include <string>
#include "mr_buffer.h"
#include "mr_texture.h"

namespace mr
{

class Mesh
{
public:
    typedef struct
    {
        Buffer *vertexBuffer;
        Buffer *indexBuffer;
        std::unordered_map<std::string,Texture*> &textures;
    } CreateParams;

public:
    static Mesh *Create(CreateParams &params);
    virtual ~Mesh() = 0 {};

protected:
    Buffer *vertexBuffer;
    Buffer *indexBuffer;
    std::unordered_map<std::string,Texture*> textures;
};

}

#endif