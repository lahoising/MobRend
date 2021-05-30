#ifndef _MR_ASSET_MANAGER_H_
#define _MR_ASSET_MANAGER_H_

#include <vector>
#include "mr_singleton.h"

namespace mr
{
    
class AssetManager
{
public:
    ~AssetManager(){}

    /// TODO: replace std::vector<char> to an array object we define
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