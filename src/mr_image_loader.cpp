#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "mr_image_loader.h"

namespace mr
{
    
unsigned char *ImageLoader::Load(const char *filepath, ImageData *dataOutput)
{
    return dataOutput?  stbi_load(filepath, &dataOutput->width, &dataOutput->height, &dataOutput->nrChannels, 0) :
                        stbi_load(filepath, nullptr, nullptr, nullptr, 0);
}

void ImageLoader::DeleteImage(unsigned char *imageData)
{
    stbi_image_free(imageData);
}

} // namespace mr
