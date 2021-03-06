#include <queue>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mr_logger.h"
#include <mobrend/model.h>

namespace mr
{
    
    namespace ModelLoader
    {
        static Model *Load(const char *filepath);
        static std::vector<Mesh*> ProcessNode(aiNode *rootNode, const aiScene *scene);
        static Mesh *ProcessMesh(aiMesh *mesh, const aiScene *scene);
    } // namespace ModelLoader
    
Model::Model()
    : meshes() {}

Model::Model(const std::vector<Mesh*> &meshes)
    : meshes(meshes){}

Model::~Model()
{
    for(Mesh *mesh : this->meshes)
    {
        delete(mesh);
    }
    this->meshes.clear();
}

Model *Model::Load(const char *filepath)
{
    return ModelLoader::Load(filepath);
}

Model *ModelLoader::Load(const char *filepath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    MR_ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode);
    auto meshes = ModelLoader::ProcessNode(scene->mRootNode, scene);

    return new Model(meshes);
}

std::vector<Mesh*> ModelLoader::ProcessNode(aiNode *rootNode, const aiScene *scene)
{
    std::queue<aiNode*> nodesQueue;
    nodesQueue.push(rootNode);

    std::vector<Mesh*> meshes;
    while(!nodesQueue.empty())
    {
        aiNode *node = nodesQueue.front();
        nodesQueue.pop();

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            nodesQueue.push(node->mChildren[i]);
        }

        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            meshes.push_back(ModelLoader::ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
        }
    }

    return meshes;
}

Mesh *ModelLoader::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    enum VertexAttributeType
    {
        POSITION,
        NORMAL,
        TEXCOORD
    };

    std::vector<VertexAttributeType> vertAttributes;
    VertexLayout layout = {};
    if(mesh->HasPositions())
    {
        layout.PushAttribute({AttributeType::ATTRIBUTE_TYPE_FLOAT, 3});
        vertAttributes.push_back(POSITION);
    }
    if(mesh->HasNormals())
    {
        layout.PushAttribute({AttributeType::ATTRIBUTE_TYPE_FLOAT, 3});
        vertAttributes.push_back(NORMAL);
    }
    for(unsigned int i = 0; mesh->HasTextureCoords(i); i++)
    {
        layout.PushAttribute({AttributeType::ATTRIBUTE_TYPE_FLOAT, 2});
        vertAttributes.push_back(TEXCOORD);
    }

    std::vector<char> buffer;
    buffer.resize(mesh->mNumVertices * layout.GetStride());
    char *bufferPtr = buffer.data();
    
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        int texCoordsCount = 0;
        for(auto &attribType : vertAttributes)
        {
            switch (attribType)
            {
            case POSITION:
            {
                auto vert = mesh->mVertices[i];
                memcpy(bufferPtr, glm::value_ptr(glm::vec3(vert.x, vert.y, vert.z)), sizeof(glm::vec3));
                bufferPtr += sizeof(glm::vec3);
            }
            break;

            case NORMAL:
            {
                auto norm = mesh->mNormals[i];
                memcpy(bufferPtr, glm::value_ptr(glm::vec3(norm.x, norm.y, norm.z)), sizeof(glm::vec3));
                bufferPtr += sizeof(glm::vec3);
            }
            break;
            
            case TEXCOORD:
            {
                auto texCoord = mesh->mTextureCoords[texCoordsCount++][i];
                memcpy(bufferPtr, glm::value_ptr(glm::vec2(texCoord.x, texCoord.y)), sizeof(glm::vec2));
                bufferPtr += sizeof(glm::vec2);
            }
            break;

            default:
                break;
            }
        }
    }

    std::vector<uint32_t> indices;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace &face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh::CreateParams params = {};
    params.vertices = buffer.data();
    params.verticesArraySize = buffer.size();
    params.vertexLayout = &layout;
    params.indices = indices.data();
    params.indexCount = indices.size();
    Mesh *loadedMesh = new Mesh(params);
    
    return loadedMesh;
}


} // namespace mr
