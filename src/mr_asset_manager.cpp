#include <stdio.h>
#include "mr_logger.h"
#include "mr_asset_manager.h"

namespace mr
{
    
std::vector<char> AssetManager::GetFileContent(const char *filepath)
{
    MR_ASSERT(filepath && strlen(filepath));

    FILE *file = fopen(filepath, "rb");
    MR_ASSERT(file);
    int seekRes = fseek(file, 0L, SEEK_END);
    MR_ASSERT(!seekRes);
    long fileSize = ftell(file);
    seekRes = fseek(file, 0L, SEEK_SET);
    MR_ASSERT(!seekRes);

    std::vector<char> ret;
    ret.reserve(fileSize);
    int c = 0;
    while((c = fgetc(file)) != EOF)
    {
        ret.push_back(c);
    }

    fclose(file);
    return ret;
}

} // namespace mr
