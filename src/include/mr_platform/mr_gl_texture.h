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
    GlTexture(unsigned char *imageContent, ImageData imageData);
    virtual ~GlTexture() override;
    virtual void Bind() override;
    virtual void Unbind() override;

private:
    unsigned int textureId;
};

} // namespace mr

#endif