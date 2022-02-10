#include <mobrend/material.h>

namespace mr
{
	Material::Material(Shader *shader)
		: shader(shader), data()
	{
	}

	Material::~Material()
	{
		this->shader = nullptr;
		this->info.clear();
		this->data.clear();
	}

	void Material::SetBool(const std::string &name, bool val)
	{
		this->SetValue<bool>(name, val);
	}

	bool Material::GetBool(const std::string &name)
	{
		return GetValue<bool>(name);
	}

	void Material::SetInt(const std::string &name, int32_t val)
	{
		return SetValue<int32_t>(name, val);
	}

	int32_t Material::GetInt(const std::string &name)
	{
		return this->GetValue<int32_t>(name);
	}

	void  Material::SetFloat(const std::string &name, float val)
	{
		this->SetValue<float>(name, val);
	}

	float Material::GetFloat(const std::string &name)
	{
		return this->GetValue<float>(name);
	}

	void Material::SetVec2(const std::string &name, const glm::vec2 &val)
	{
		SetValue<glm::vec2>(name, val);
	}

	glm::vec2 Material::GetVec2(const std::string &name)
	{
		return GetValue<glm::vec2>(name);
	}

	void Material::SetVec3(const std::string &name, const glm::vec3 &val)
	{
		SetValue<glm::vec3>(name, val);
	}

	glm::vec3 Material::GetVec3(const std::string &name)
	{
		return GetValue<glm::vec3>(name);
	}

	template<typename T>
	void Material::SetValue(const std::string &name, T val)
	{
		if(this->info.find(name) == this->info.end())
		{
			this->data.resize(this->data.size() + sizeof(T));
			void *ptr = (void*)(this->data.data() + this->data.size() - sizeof(T));

			Material::ValuesInfo valInfo = {};
			valInfo.dataType = GetType<T>();
			valInfo.ptr = ptr;
			this->info[name] = valInfo;

			T *d = (T*)ptr;
			*d = val;

			return;
		}

		T *ptr = (T*)this->info[name].ptr;
		*ptr = val;
	}

	template<typename T>
	T Material::GetValue(const std::string &name)
	{
		if(this->info.find(name) == this->info.end())
		{
			return T();
		}

		void *ptr = this->info[name].ptr;
		return *(T*)ptr;
	}

	template<>
	Material::DataType Material::GetType<bool>()
	{ return MAT_DATA_TYPE_BOOL; }

	template<>
	Material::DataType Material::GetType<int32_t>()
	{ return MAT_DATA_TYPE_INT; }

	template<>
	Material::DataType Material::GetType<float>()
	{ return MAT_DATA_TYPE_FLOAT; }

	template<>
	Material::DataType Material::GetType<glm::vec2>()
	{ return MAT_DATA_TYPE_VEC2; }

	template<>
	Material::DataType Material::GetType<glm::vec3>()
	{ return MAT_DATA_TYPE_VEC3; }
}
