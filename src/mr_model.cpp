#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mr_model.h"

namespace mr
{
    
    namespace ModelLoader
    {
        static Model *Load(const char *filepath);
        static void ProcessNode(aiNode *node, const aiScene *scene);
        static void ProcessMesh(aiMesh *mesh, const aiScene *scene);
    } // namespace ModelLoader
    
Model::Model()
    : meshes() {}

Model::Model(const std::vector<Mesh*> &meshes)
    : meshes(meshes){}

Model::~Model()
{

}

Model *Model::Load(const char *filepath)
{
    return ModelLoader::Load(filepath);
}

Model *ModelLoader::Load(const char *filepath)
{
    Model *model = new Model();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    return model;
}

void ModelLoader::ProcessNode(aiNode *node, const aiScene *scene)
{

}

void ModelLoader::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{

}


} // namespace mr