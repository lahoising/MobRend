#ifndef _MR_TEXTURE_H_
#define _MR_TEXTURE_H_

#include <inttypes.h>
#include <unordered_map>

namespace mr
{
    
class Texture
{
public:
    static Texture *Create(const char *filepath);
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