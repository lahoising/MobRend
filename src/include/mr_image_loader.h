#ifndef _MR_IMAGE_LOADER_H_
#define _MR_IMAGE_LOADER_H_

namespace mr
{
    
typedef struct
{
    int width, height;
    int nrChannels;
} ImageData;

class ImageLoader
{
public:
    static unsigned char *Load(const char *filepath, ImageData *dataOutput, bool inverted = true);
    static void DeleteImage(unsigned char *imageData);
};

} // namespace mr

#endif