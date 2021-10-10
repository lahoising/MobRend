#ifndef _MR_TEXTURE_H_
#define _MR_TEXTURE_H_

#include <inttypes.h>
#include <unordered_map>
#include <mobrend/asset_manager.h>
#include <mobrend/image_loader.h>

namespace mr
{

class Texture
{
public:
    enum Format
    {
        TEXTURE_FORMAT_RGB,
        TEXTURE_FORMAT_RGBA,
        TEXTURE_FORMAT_DEPTH,
        TEXTURE_FORMAT_DEPTH_STENCIL
    };

    enum Type
    {
        TEXTURE_TYPE_2D,
        TEXTURE_TYPE_CUBE,
    };

    struct Specs
    {
        ImageData info;
        const void *content;
        Format format;
    };

    struct CubeSpecs
    {
        Specs right;
        Specs left;
        Specs top;
        Specs bottom;
        Specs front;
        Specs back;
    };
    
    struct CreateParams
    {
        union
        {
            const Specs *specs;
            const CubeSpecs *cubeSpecs;
        };
        std::string referenceName;
        Type type;

        CreateParams() :
            specs(nullptr), referenceName(""), type(TEXTURE_TYPE_2D)
            {}
    };

    struct CubePaths
    {
        char right[MR_MAX_PATH];
        char left[MR_MAX_PATH];
        char top[MR_MAX_PATH];
        char bottom[MR_MAX_PATH];
        char front[MR_MAX_PATH];
        char back[MR_MAX_PATH];
    };

    struct LoadParams
    {

        union
        {
            const char *filepath;
            const CubePaths *cubeMapPaths;
        };
        Type type;
        bool invertVertically;

        LoadParams() :
        filepath(nullptr), type(TEXTURE_TYPE_2D),
        invertVertically(true)
        {}
    };

public:
    static Texture *Load(const LoadParams &params);
    static Texture *Create(const CreateParams &params);
    virtual ~Texture() = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    uint32_t GetWidth(){ return this->w; }
    uint32_t GetHeight(){ return this->h; }

protected:
    uint32_t w, h;
};

class TextureManager
{
public:
    static TextureManager &GetInstance();
    ~TextureManager();
    inline void AddTexture(const char *filepath, Texture *texture)
    {
        this->loadedTextures[filepath] = texture;
    }
    inline Texture *GetTexture(const char *filepath)
    {
        return this->loadedTextures[filepath];
    }
    bool Contains(const char *filepath);

private:
    TextureManager();

private:
    std::unordered_map<std::string,Texture*> loadedTextures;
};


} // namespace mr

#endif
