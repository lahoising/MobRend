#ifndef _MR_MATERIAL_H_
#define _MR_MATERIAL_H_

#include <bits/stdint-intn.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
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
		MAT_DATA_TYPE_VEC2,
		MAT_DATA_TYPE_VEC3,
		MAT_DATA_TYPE_VEC4,
		MAT_DATA_TYPE_MAT3,
		MAT_DATA_TYPE_MAT4,
		MAT_DATA_TYPE_TEXTURE,
		MAT_DATA_TYPE_UNIFORM_BUFFER,
	};

private:
	struct ValuesInfo
	{
		DataType dataType;
		std::size_t ptrOffset;
	};

private:
	mr::Shader *shader;
	std::unordered_map<std::string,ValuesInfo> info;
	std::vector<char> data;

public:
	Material(mr::Shader *shader);
	~Material();

	void SetBool(const std::string &name, bool val);
	bool GetBool(const std::string &name);

	void SetInt(const std::string &name, int32_t val);
	int32_t GetInt(const std::string &name);

	void SetFloat(const std::string &name, float val);
	float GetFloat(const std::string &name);

	void SetVec2(const std::string &name, const glm::vec2 &val);
	glm::vec2 GetVec2(const std::string &name);

	void SetVec3(const std::string &name,const glm::vec3 &val);
	glm::vec3 GetVec3(const std::string &name);

	void SetVec4(const std::string &name, const glm::vec4 &val);
	glm::vec4 GetVec4(const std::string &name);

	void SetMat3(const std::string &name, const glm::mat3 &val);
	glm::mat3 GetMat3(const std::string &name);

	void SetMat4(const std::string &name, const glm::mat4 &val);
	glm::mat4 GetMat4(const std::string &name);

	// MAT_DATA_TYPE_TEXTURE,
	void SetTexture(const std::string &name, const mr::Texture *tex);
	const mr::Texture *GetTexture(const std::string &name);

	// MAT_DATA_TYPE_UNIFORM_BUFFER,
	void SetUniformBuffer(const std::string &name, const mr::UniformBuffer *ubo);
	const mr::UniformBuffer *GetUniformBuffer(const std::string &name);

	void Bind();

	mr::Shader *GetShader(){ return this->shader; }

private:
	template<typename T>
	void SetValue(const std::string &name, T val);

	template<typename T>
	T GetValue(const std::string &name);

	template<typename T>
	static DataType GetType();
};

} // namespace mr

#endif
