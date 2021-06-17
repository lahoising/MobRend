#ifndef _MR_TEXTURE_H_
#define _MR_TEXTURE_H_

#include <inttypes.h>
#include <unordered_map>
#include "mr_image_loader.h"

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
    
    struct CreateParams
    {
        ImageData info;
        std::string referenceName;
        const void *content;
        Format format;
    };

public:
    static Texture *Load(const char *filepath);
    static Texture *Create(const CreateParams &params);
    virtual ~Texture() = 0 {};
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