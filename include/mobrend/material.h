#ifndef _MR_MATERIAL_H_
#define _MR_MATERIAL_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <mobrend/shader.h>

namespace mr
{
    
class Material
{
public:
	enum DataType
	{
		MAT_DATA_TYPE_BOOL,
		MAT_DATA_TYPE_INT,
		MAT_DATA_TYPE_FLOAT,
		MAT_DATA_TYPE_FLOAT2,
		MAT_DATA_TYPE_FLOAT3,
		MAT_DATA_TYPE_FLOAT4,
		MAT_DATA_TYPE_MAT3,
		MAT_DATA_TYPE_MAT4,
		MAT_DATA_TYPE_TEXTURE,
		MAT_DATA_TYPE_UNIFORM_BUFFER,
	};

private:
	struct ValuesInfo
	{
		DataType dataType;
		void *ptr;
	};

private:
	mr::Shader *shader;
	std::unordered_map<std::string,ValuesInfo> info;
	std::vector<char> data;

public:
	Material(mr::Shader *shader);
	~Material();

	mr::Shader *GetShader(){ return this->shader; }
};

} // namespace mr

#endif
