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
    params.info = data;
    
    switch (params.info.nrChannels)
    {
    case 3: params.format = Texture::TEXTURE_FORMAT_RGB; break;
    case 4: params.format = Texture::TEXTURE_FORMAT_RGBA; break;
    default: break;
    }

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

    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GlTexture::GetInternalFormat(params.format),
        params.info.width, params.info.height,
        0, 
        GlTexture::GetFormat(params.format),
        GlTexture::GetContentType(params.format),
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

unsigned int GlTexture::GetInternalFormat(Texture::Format format)
{
    switch (format)
    {
    case TEXTURE_FORMAT_RGB:            return GL_RGB;
    case TEXTURE_FORMAT_RGBA:           return GL_RGBA;
    case TEXTURE_FORMAT_DEPTH:          return GL_DEPTH_COMPONENT32;
    case TEXTURE_FORMAT_DEPTH_STENCIL:  return GL_DEPTH_STENCIL;
    default: throw "Unknown texture format";
    }
}

unsigned int GlTexture::GetFormat(Texture::Format format)
{
    switch (format)
    {
    case TEXTURE_FORMAT_RGB:            return GL_RGB;
    case TEXTURE_FORMAT_RGBA:           return GL_RGBA;
    case TEXTURE_FORMAT_DEPTH:          return GL_DEPTH_COMPONENT;
    case TEXTURE_FORMAT_DEPTH_STENCIL:  return GL_DEPTH24_STENCIL8;
    default: throw "Unknown texture format";
    }
}

unsigned int GlTexture::GetContentType(Texture::Format format)
{
    switch (format)
    {
    case TEXTURE_FORMAT_RGB:
    case TEXTURE_FORMAT_RGBA:
        return GL_UNSIGNED_BYTE;
    case TEXTURE_FORMAT_DEPTH: return GL_UNSIGNED_INT;
    case TEXTURE_FORMAT_DEPTH_STENCIL: return GL_UNSIGNED_INT_24_8;
    default: throw "Unknown texture format";
    }
}

} // namespace mr
