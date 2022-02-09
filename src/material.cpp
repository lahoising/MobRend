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
}
