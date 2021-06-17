#include <glad/glad.h>
#include "mr_logger.h"
#include "mr_platform/mr_gl_texture.h"

namespace mr
{
    
GlTexture::GlTexture(const Texture::CreateParams &params)
{
    this->GenerateTexture(params);
}

GlTexture::GlTexture(const char *filepath)
{
    ImageData data = {};
    unsigned char *imageData = ImageLoader::Load(filepath, &data);

    Texture::CreateParams params = {};
    params.content = imageData;
    params.format = data;

    this->GenerateTexture(params);
    ImageLoader::DeleteImage((unsigned char *)params.content);

    mrlog("tex id: %u", this->textureId);
}

void GlTexture::GenerateTexture(const Texture::CreateParams &params)
{
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int format = 0;
    switch (params.format.nrChannels)
    {
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default: break;
    }

    glTexImage2D(
        GL_TEXTURE_2D, 0,
        format,
        params.format.width, params.format.height,
        0, 
        format, GL_UNSIGNED_BYTE,
        params.content
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    this->Unbind();
}

GlTexture::~GlTexture()
{
    glDeleteTextures(1, &this->textureId);
}

void GlTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, this->textureId);
}

void GlTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


} // namespace mr
