#include "mr_texture.h"

#ifdef MOBREND_GL_RENDERING
#include "mr_platform/mr_gl_texture.h"
#endif

namespace mr
{

Texture *Texture::Create(const CreateParams &params)
{
    TextureManager &manager = TextureManager::GetInstance();
    if(manager.Contains(params.referenceName.c_str()))
    {
        return manager.GetTexture(params.referenceName.c_str());
    }
    
    Texture *ret = nullptr;
    #ifdef MOBREND_GL_RENDERING
    ret = new GlTexture(params);
    #else
    ret = nullptr;
    #endif

    manager.AddTexture(params.referenceName.c_str(), ret);
    return ret;
}
    
Texture *Texture::Load(const LoadParams &params)
{
    TextureManager &manager = TextureManager::GetInstance();
    if(manager.Contains(params.filepath))
    {
        return manager.GetTexture(params.filepath);
    }
    
    Texture *ret = nullptr;
    #ifdef MOBREND_GL_RENDERING
    ret = new GlTexture(params);
    #else
    ret = nullptr;
    #endif

    manager.AddTexture(params.filepath, ret);
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
