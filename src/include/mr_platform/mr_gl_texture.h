#ifndef _MR_GL_TEXTURE_H_
#define _MR_GL_TEXTURE_H_

#include "mr_image_loader.h"
#include "mr_texture.h"

namespace mr
{
    
class GlTexture : public Texture
{
public:
    GlTexture(const char *filepath);
    GlTexture(const Texture::CreateParams &params);
    void GenerateTexture(const Texture::CreateParams &params);
    virtual ~GlTexture() override;
    virtual void Bind() override;
    virtual void Unbind() override;

    unsigned int GetTextureId(){ return this->textureId; }

private:
    static unsigned int GetFormat(Texture::Format format);
    static unsigned int GetInternalFormat(Texture::Format format);
    static unsigned int GetContentType(Texture::Format format);
    static unsigned int GetTextureType(Texture::Type type);

private:
    typedef struct
    {
        void (*generate)(GlTexture *texture, const GlTexture::CreateParams &params);
        unsigned int type;
    } GeneratorInfo;

    static GeneratorInfo BuildGeneratorInfo(GlTexture::Type type);
    static void GenerateTexture2D(GlTexture *texture, const GlTexture::CreateParams &params);

private:
    unsigned int textureId;
    // unsigned char *imageContent;
};

} // namespace mr

#endif