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
		if(this->info.find(name) == this->info.end())
		{
			this->data.resize(this->data.size() + sizeof(bool));
			void *ptr = (void*)(this->data.data() + this->data.size() - sizeof(bool));

			Material::ValuesInfo valInfo = {};
			valInfo.dataType = MAT_DATA_TYPE_BOOL;
			valInfo.ptr = ptr;
			this->info[name] = valInfo;

			bool *d = (bool*)ptr;
			*d = val;

			return;
		}

		bool *ptr = (bool*)this->info[name].ptr;
		*ptr = val;
	}

	bool Material::GetBool(const std::string &name)
	{
		if(this->info.find(name) == this->info.end())
		{
			return false;
		}

		void *ptr = this->info[name].ptr;
		return *(bool*)ptr;
	}
}
