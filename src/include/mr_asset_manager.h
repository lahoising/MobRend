#ifndef _MR_ASSET_MANAGER_H_
#define _MR_ASSET_MANAGER_H_

#include <vector>

namespace mr
{
    
class AssetManager
{
public:
    ~AssetManager(){}

    /// TODO: replace std::vector<char> to a data type that doesn't allocate in heap
    std::vector<char> GetFileContent(const char *filepath);

private:
    AssetManager(){}

public:
    static AssetManager &GetInstance()
    {
        static AssetManager manager = AssetManager();
        return manager;
    }
};

} // namespace mr

#endif