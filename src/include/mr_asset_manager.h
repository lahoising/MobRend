#ifndef _MR_ASSET_MANAGER_H_
#define _MR_ASSET_MANAGER_H_

#include <vector>
#include <cstring>
#include <string>
#include <inttypes.h>

#define MR_MAX_PATH 260

namespace mr
{
    
class AssetManager
{
public:
    ~AssetManager(){}

    /// TODO: replace std::vector<char> to a data type that doesn't allocate in heap
    std::vector<char> GetFileContent(const char *filepath);
    std::vector<uint32_t> GetFileContentInBinary(const char *filePath);
    
    inline static void GetAssetPath(char *dst, const char *path)
    {
        snprintf(dst, MR_MAX_PATH, "%s%s", MOBREND_ASSETS_RELATIVE_PATH, path);
    }

private:
    AssetManager(){}
    FILE *OpenFile(const char *filepath, long &outSize);

public:
    static AssetManager &GetInstance()
    {
        static AssetManager manager = AssetManager();
        return manager;
    }
};

} // namespace mr

#endif