#include <glad/glad.h>
#include "mr_logger.h"
#include "mr_platform/mr_gl_texture.h"

namespace mr
{

GlTexture::GlTexture(const Texture::CreateParams &params)
{
    this->GenerateTexture(params);
}

GlTexture::GlTexture(const LoadParams &params)
{
    Texture::CreateParams createParams = {};
    createParams.type = params.type;

    switch (params.type)
    {
    case GlTexture::TEXTURE_TYPE_2D:
    {
        Specs specs = GlTexture::CreateSpecs(params.filepath);
        createParams.specs = &specs;
        createParams.referenceName = params.filepath;

        this->GenerateTexture(createParams);
        ImageLoader::DeleteImage((unsigned char *)specs.content);
    }
    break;
    
    case GlTexture::TEXTURE_TYPE_CUBE:
    {
        CubeSpecs specs = {};
        specs.right = GlTexture::CreateSpecs(params.cubeMapPaths->right);
        specs.left = GlTexture::CreateSpecs(params.cubeMapPaths->left);
        specs.top = GlTexture::CreateSpecs(params.cubeMapPaths->top);
        specs.bottom = GlTexture::CreateSpecs(params.cubeMapPaths->bottom);
        specs.front = GlTexture::CreateSpecs(params.cubeMapPaths->front);
        specs.back = GlTexture::CreateSpecs(params.cubeMapPaths->back);

        createParams.cubeSpecs = &specs;
        createParams.referenceName = "cube_map_";
        createParams.referenceName += params.cubeMapPaths->right;

        this->GenerateTexture(createParams);

        ImageLoader::DeleteImage((unsigned char *)specs.right.content);
        ImageLoader::DeleteImage((unsigned char *)specs.left.content);
        ImageLoader::DeleteImage((unsigned char *)specs.top.content);
        ImageLoader::DeleteImage((unsigned char *)specs.bottom.content);
        ImageLoader::DeleteImage((unsigned char *)specs.front.content);
        ImageLoader::DeleteImage((unsigned char *)specs.back.content);
    }
    break;

    default: throw "Unknown texture type";
    }

    mrlog("tex id: %u", this->textureId);
}

Texture::Specs GlTexture::CreateSpecs(const char *filepath)
{
    ImageData data = {};
    unsigned char *imageData = ImageLoader::Load(filepath, &data);

    Texture::Specs specs = {};
    specs.content = imageData;
    specs.info = data;
    // default type is texture 2d
    
    switch (specs.info.nrChannels)
    {
    case 3: specs.format = Texture::TEXTURE_FORMAT_RGB; break;
    case 4: specs.format = Texture::TEXTURE_FORMAT_RGBA; break;
    default: break;
    }

    return specs;
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
        GlTexture::GetInternalFormat(params.specs->format),
        params.specs->info.width, params.specs->info.height,
        0, 
        GlTexture::GetFormat(params.specs->format),
        GlTexture::GetContentType(params.specs->format),
        params.specs->content
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
