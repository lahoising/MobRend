#ifndef _MR_MODEL_H_
#define _MR_MODEL_H_

#include <vector>
#include <string>
#include "mr_mesh.h"

namespace mr
{
    
class Model
{
public:
    static Model *Load(const char *filepath);
    Model();
    Model(const std::vector<Mesh*> &meshes);
    ~Model();

    const std::vector<Mesh*> &GetMeshes(){ return this->meshes; };

private:
    std::vector<Mesh*> meshes;
};

} // namespace mr

#endif