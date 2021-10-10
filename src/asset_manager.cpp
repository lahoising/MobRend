#include <stdio.h>
#include <cstring>
#include <string>
#include <algorithm>
#include "mr_logger.h"
#include <mobrend/asset_manager.h>

namespace mr
{
    
std::vector<char> AssetManager::GetFileContent(const char *filepath)
{
    MR_ASSERT(filepath && strlen(filepath));

    // FILE *file = fopen(filepath, "rb");
    // MR_ASSERT(file);
    // int seekRes = fseek(file, 0L, SEEK_END);
    // MR_ASSERT(!seekRes);
    // long fileSize = ftell(file);
    // seekRes = fseek(file, 0L, SEEK_SET);
    // MR_ASSERT(!seekRes);

    long fileSize = 0;
    FILE* file = this->OpenFile(filepath, fileSize);

    std::vector<char> ret;
    ret.resize(fileSize);
    int c = 0;
    while((c = fgetc(file)) != EOF)
    {
        ret.push_back(c);
    }

    fclose(file);
    return ret;
}

std::vector<uint32_t> AssetManager::GetFileContentInBinary(const char *filePath)
{
    long fileSize = 0;
    FILE *file = this->OpenFile(filePath, fileSize);

    std::vector<uint32_t> ret;
    ret.resize(fileSize/sizeof(uint32_t));
    fread(ret.data(), fileSize, 1, file);
    fclose(file);

    return ret;
}

FILE *AssetManager::OpenFile(const char *filepath, long &outSize)
{
    MR_ASSERT(filepath && strlen(filepath));

#if defined(WIN32)
    std::string osSpecificFilepath = filepath;
    std::replace( osSpecificFilepath.begin(), osSpecificFilepath.end(), '/', '\\');
    filepath = osSpecificFilepath.c_str();
#endif
    FILE *file = fopen(filepath, "rb");
    MR_ASSERT(file);
    int seekRes = fseek(file, 0L, SEEK_END);
    MR_ASSERT(!seekRes);
    outSize = ftell(file);
    seekRes = fseek(file, 0L, SEEK_SET);
    MR_ASSERT(!seekRes);

    return file;
}

} // namespace mr
