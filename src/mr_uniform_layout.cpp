#include <ctype.h>
#include <memory>
#include "mr_uniform_layout.h"

namespace mr
{

/// @note source is assumed to be at the beginning of a glsl type
static void GetNextWord(const char **source, std::string &outWord)
{
    const char *nextSpace = *source;
    while(!isspace(*nextSpace)) nextSpace++;

    outWord.assign(*source, nextSpace - *source);
    *source = nextSpace;
}

UniformLayout GetUniformLayout(const char *source)
{
    UniformLayout uniforms;

    const size_t uniformWordLen = strlen("uniform");
    const size_t structWordLen = strlen("struct");
    const char *scanner = source;
    std::string buffer = "";
    buffer.reserve(strlen(scanner));

    std::unordered_map<std::string,UniformLayout> shaderStructs;
    std::string structName = "";
    structName.reserve(128);
    while(scanner = strstr(scanner, "struct"))
    {
        UniformLayout structLayout;

        scanner += structWordLen;
        while(isspace(*scanner)) scanner++;
        GetNextWord(&scanner, structName);

        while(isspace(*scanner) || *scanner == '{') scanner++;
        // GetNextWord(&scanner, )
    }

    scanner = source;
    // while(scanner = strstr(scanner, "uniform"))
    // {
    //     scanner += uniformWordLen;
    //     while(isspace(*scanner)) scanner++;

        
    // }
    return uniforms;
}

} // namespace mr
