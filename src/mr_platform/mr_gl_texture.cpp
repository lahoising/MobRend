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
    // default type is texture 2d
    
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
    GeneratorInfo info = GlTexture::BuildGeneratorInfo(params.type);
    glGenTextures(1, &this->textureId);
    glBindTexture(info.type, this->textureId);

    glTexParameteri(info.type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(info.type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(info.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(info.type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    info.generate(this,params);
    glGenerateMipmap(info.type);

    this->Unbind();
}

GlTexture::GeneratorInfo GlTexture::BuildGeneratorInfo(GlTexture::Type type)
{
    GlTexture::GeneratorInfo info = {};
    info.type = GlTexture::GetTextureType(type);
    
    switch (type)
    {
    case GlTexture::TEXTURE_TYPE_2D: 
        info.generate = GlTexture::GenerateTexture2D;
    break;
    default: throw "Unknown texture type";
    }

    return info;
}

void GlTexture::GenerateTexture2D(GlTexture *texture, const GlTexture::CreateParams &params)
{
    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GlTexture::GetInternalFormat(params.format),
        params.info.width, params.info.height,
        0, 
        GlTexture::GetFormat(params.format),
        GlTexture::GetContentType(params.format),
        params.content
    );
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

unsigned int GlTexture::GetTextureType(Texture::Type type)
{
    switch (type)
    {
    case TEXTURE_TYPE_2D:    return GL_TEXTURE_2D;
    case TEXTURE_TYPE_CUBE:  return GL_TEXTURE_CUBE_MAP;
    default: throw "Unknown texture type";
    }
}

} // namespace mr
