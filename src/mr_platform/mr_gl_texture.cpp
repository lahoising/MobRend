#include <glad/glad.h>
#include "mr_platform/mr_gl_texture.h"

namespace mr
{
    
GlTexture::GlTexture(unsigned char *imageContent, ImageData imageData)
{
    if(imageContent)
    {
        glGenTextures(1, &this->textureId);
        glBindTexture(GL_TEXTURE_2D, this->textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0,
            GL_RGB,
            imageData.width, imageData.height,
            0, 
            GL_RGB, GL_UNSIGNED_BYTE,
            imageContent
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        ImageLoader::DeleteImage(imageContent);
    }
    else
    {

    }
}

GlTexture::GlTexture(const char *filepath)
{
    ImageData data = {};
    unsigned char *imageData = ImageLoader::Load(filepath, &data);
    GlTexture(imageData, data);
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
