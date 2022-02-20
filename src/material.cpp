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

	void Material::SetVec4(const std::string &name, const glm::vec4 &val)
	{
		SetValue<glm::vec4>(name, val);
	}

	glm::vec4 Material::GetVec4(const std::string &name)
	{
		return GetValue<glm::vec4>(name);
	}

	void Material::SetMat3(const std::string &name, const glm::mat3 &val)
	{
		SetValue<glm::mat3>(name, val);
	}

	glm::mat3 Material::GetMat3(const std::string &name)
	{
		return GetValue<glm::mat3>(name);
	}

	void Material::SetMat4(const std::string &name, const glm::mat4 &val)
	{
		SetValue<glm::mat4>(name, val);
	}

	glm::mat4 Material::GetMat4(const std::string &name)
	{
		return GetValue<glm::mat4>(name);
	}

	void Material::SetTexture(const std::string &name, const mr::Texture *tex)
	{
		SetValue<const Texture*>(name, tex);
	}

	const mr::Texture *Material::GetTexture(const std::string &name)
	{
		return GetValue<const mr::Texture*>(name);
	}

	void Material::SetUniformBuffer(const std::string &name, const mr::UniformBuffer *ubo)
	{
		SetValue<const mr::UniformBuffer*>(name, ubo);
	}

	const mr::UniformBuffer *Material::GetUniformBuffer(const std::string &name)
	{
		return GetValue<const mr::UniformBuffer*>(name);
	}

	void Material::Bind()
	{
		this->shader->Bind();
		for(auto valsInfo : this->info)
		{
			const ValuesInfo &i = valsInfo.second;
			const void *ptr = this->data.data() + i.ptrOffset;
			switch(i.dataType)
			{
			case MAT_DATA_TYPE_FLOAT:
				this->shader->UploadFloat(
					valsInfo.first.c_str(),
					*((float*)ptr));
				break;
			case MAT_DATA_TYPE_VEC3:
				this->shader->UploadVec3(
					valsInfo.first.c_str(),
					*((glm::vec3*)ptr));
				break;
			case MAT_DATA_TYPE_VEC4:
				this->shader->UploadVec4(
					valsInfo.first.c_str(),
					*((glm::vec4*)ptr));
				break;
			case MAT_DATA_TYPE_TEXTURE:
				this->shader->UploadTexture(
					valsInfo.first.c_str(),
					*((mr::Texture**)ptr));
				break;
			default:
				break;
			}
		}
	}

	template<typename T>
	void Material::SetValue(const std::string &name, T val)
	{
		if(this->info.find(name) == this->info.end())
		{
			const std::size_t dataSize = this->data.size();
			this->data.resize(dataSize + sizeof(T));

			Material::ValuesInfo valInfo = {};
			valInfo.dataType = GetType<T>();
			valInfo.ptrOffset = dataSize;
			this->info[name] = valInfo;

			T *d = (T*)(this->data.data() + dataSize);
			*d = val;

			return;
		}

		T *ptr = (T*)(this->data.data() + this->info[name].ptrOffset);
		*ptr = val;
	}

	template<typename T>
	T Material::GetValue(const std::string &name)
	{
		if(this->info.find(name) == this->info.end())
		{
			return T();
		}

		void *ptr = this->data.data() + this->info[name].ptrOffset;
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

	template<>
	Material::DataType Material::GetType<glm::vec4>()
	{ return MAT_DATA_TYPE_VEC4; }

	template<>
	Material::DataType Material::GetType<glm::mat3>()
	{ return MAT_DATA_TYPE_MAT3; }

	template<>
	Material::DataType Material::GetType<glm::mat4>()
	{ return MAT_DATA_TYPE_MAT4; }

	template<>
	Material::DataType Material::GetType<const Texture*>()
	{ return MAT_DATA_TYPE_TEXTURE; }

	template<>
	Material::DataType Material::GetType<const UniformBuffer*>()
	{ return MAT_DATA_TYPE_UNIFORM_BUFFER; }
}
