#include <ctype.h>
#include <memory>
#include "mr_uniform_layout.h"

namespace mr
{

/// @note source is assumed to be at the beginning of a glsl type
static void GetNextWord(const char **source, std::string &outWord)
{
    const char *nextSpace = *source;
    while(isalnum(*nextSpace)) nextSpace++;

    outWord.assign(*source, nextSpace - *source);
    *source = nextSpace;
}

static UniformType IdentifyUniformType(const std::string &word)
{
    const char *scanner = word.data();

    switch (*scanner)
    {
    case 'v': // vec of some kind
        if(strstr(scanner, "vec"))
        {
            switch (*(scanner + 3))
            {
            case '2': return UniformType::VEC2;
            case '3': return UniformType::VEC3;
            case '4': return UniformType::VEC4;
            default: return UniformType::UDSTRUCT;
            }
        }
    break;
    
    default: return UniformType::UDSTRUCT;
    }
}

UniformLayout GetUniformLayout(const char *source)
{
    UniformLayout uniforms;

    const size_t uniformWordLen = strlen("uniform");
    const size_t structWordLen = strlen("struct");
    const char *scanner = source;
    
    std::string buffer = "";
    buffer.reserve(strlen(scanner));
    std::string variableName = "";
    variableName.reserve(64); // too much, but whatever

    std::unordered_map<std::string,UniformLayout> shaderStructs;
    std::string structName = "";
    structName.reserve(128);
    while(scanner = strstr(scanner, "struct"))
    {
        UniformLayout structLayout;

        /// TODO: what if struct is in the name of a variable?
        scanner += structWordLen;
        while(isspace(*scanner)) scanner++;
        GetNextWord(&scanner, structName);

        while(isspace(*scanner) || *scanner == '{') scanner++;

        UniformValue value = {};

        // at this point we are at a line and the next two words will be:
        // 1. the uniform type
        // 2. the name of the variable
        GetNextWord(&scanner, buffer);
        value.type = IdentifyUniformType(buffer);

        while(isspace(*scanner)) scanner++;
        GetNextWord(&scanner, variableName);

        while(isspace(*scanner)) scanner++;
        if(*scanner == '[')
        {
            
        }
        
        structLayout[variableName] = value;
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
