#include "mr_texture.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_texture.h"
#endif

namespace mr
{
    
Texture *Texture::Create(const char *filepath)
{
    TextureManager &manager = TextureManager::GetInstance();
    if(manager.Contains(filepath))
    {
        return manager.GetTexture(filepath);
    }
    
    Texture *ret = nullptr;
    #ifdef MOBREND_GL_RENDERING
    ret = new GlTexture(filepath);
    #else
    ret = nullptr;
    #endif

    manager.AddTexture(filepath, ret);
    return ret;
}

bool TextureManager::Contains(const char *filepath)
{
    return this->loadedTextures.find(filepath) != this->loadedTextures.end();
}

TextureManager &TextureManager::GetInstance()
{
    static TextureManager manager = TextureManager();
    return manager;
}

TextureManager::TextureManager()
    : loadedTextures() {}

TextureManager::~TextureManager()
{
    for(auto &kv : this->loadedTextures)
    {
        delete(kv.second);
    }
    this->loadedTextures.clear();
}

} // namespace mr
