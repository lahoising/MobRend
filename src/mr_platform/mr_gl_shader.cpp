#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spirv_glsl.hpp>

#include "mr_logger.h"
#include "mr_platform/mr_gl_shader.h"
#include "mr_texture.h"
#include "mr_asset_manager.h"
#include "mr_uniform_layout.h"
#include "mr_platform/mr_gl_uniform_buffer.h"

namespace mr
{
    
GlShader::GlShader(Shader::CreateParams params)
	: textureSlotsCount(GL_TEXTURE0)
{
    // Create the shaders
	GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	/// TODO: change std string for a data structure that doesn't allocate in heap
    std::string vertexShaderCode = "";
    std::string fragmentShaderCode = "";

	AssetManager &assetManager = AssetManager::GetInstance();
	if(params.vertFilePath)
	{
		auto buffer = assetManager.GetFileContentInBinary(params.vertFilePath);
		vertexShaderCode.assign(
			this->CompileFromSpirV(buffer)
		);
	}

	if(params.fragFilePath)
	{
		auto buffer = assetManager.GetFileContentInBinary(params.fragFilePath);
		fragmentShaderCode.assign(
			this->CompileFromSpirV(buffer)
		);
	}

	GetUniformLayout(vertexShaderCode.c_str());
	GetUniformLayout(fragmentShaderCode.c_str());

	this->CompileShader(vertShaderId, vertexShaderCode.c_str());
	this->CompileShader(fragShaderId, fragmentShaderCode.c_str());

	// Link the program
	this->programId = glCreateProgram();
	glAttachShader(this->programId, vertShaderId);
	glAttachShader(this->programId, fragShaderId);
	glLinkProgram(this->programId);

	GLint result = GL_FALSE;
	int infoLogLength;

	// Check the program
	glGetProgramiv(this->programId, GL_LINK_STATUS, &result);
	glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(this->programId, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}
	
	glDetachShader(this->programId, vertShaderId);
	glDetachShader(this->programId, fragShaderId);
	
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);
}

GlShader::~GlShader()
{
    glDeleteProgram(this->programId);
}

std::string GlShader::CompileFromSpirV(const std::vector<uint32_t> &source)
{
	spirv_cross::CompilerGLSL glsl(std::move(source));
	spirv_cross::ShaderResources resources = glsl.get_shader_resources();

	for (auto &resource : resources.sampled_images)
	{
		unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

		// Modify the decoration to prepare it for GLSL.
		glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

		// Some arbitrary remapping if we want.
		glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
	}

	// Set some options.
	spirv_cross::CompilerGLSL::Options options;
	options.version = 410;
	options.es = false;
	glsl.set_common_options(options);

	// Compile to GLSL, ready to give to GL driver.
	return glsl.compile();
}

void GlShader::CompileShader(unsigned int shaderId, const char *shaderSource)
{
	GLint result;
	int infoLogLength;

	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> shaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, &shaderErrorMessage[0]);
		mrwarn("%s\n", &shaderErrorMessage[0]);
	}
}

void GlShader::UploadMat4(const char *uniformName,const glm::mat4 &matrix)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformName] = glGetUniformLocation(this->programId, uniformName);
	}

	const unsigned int location = this->uniformLocations[uniformName];
	glUniformMatrix4fv(
		location,
		1,
		GL_FALSE,
		glm::value_ptr(matrix)
	);
}

void GlShader::UploadMat3(const char *uniformName,const glm::mat3 &matrix)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformName] = glGetUniformLocation(this->programId, uniformName);
	}

	const unsigned int location = this->uniformLocations[uniformName];
	glUniformMatrix3fv(
		location,
		1,
		GL_FALSE,
		glm::value_ptr(matrix)
	);
}

void GlShader::UploadFloat(const char *uniformName, float val)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformName] = glGetUniformLocation(this->programId, uniformName);
	}

	const unsigned int location = this->uniformLocations[uniformName];
	glUniform1f(location, val);
}

void GlShader::UploadVec3(const char *uniformName,const glm::vec3 &vec)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformName] = glGetUniformLocation(this->programId, uniformName);
	}

	const unsigned int location = this->uniformLocations[uniformName];
	glUniform3fv(
		location,
		1,
		glm::value_ptr(vec)
	);
}

void GlShader::UploadVec4(const char *uniformName,const glm::vec4 &vec)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformName] = glGetUniformLocation(this->programId, uniformName);
	}

	const unsigned int location = this->uniformLocations[uniformName];
	glUniform4fv(
		location,
		1,
		glm::value_ptr(vec)
	);
}

void GlShader::UploadInt(const char *uniformName, int32_t i)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformName] = glGetUniformLocation(this->programId, uniformName);
	}

	const unsigned int location = this->uniformLocations[uniformName];
	glUniform1i(location, i);
}

void GlShader::UploadBool(const char *uniformName, bool val)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformName] = glGetUniformLocation(this->programId, uniformName);
	}

	const unsigned int location = this->uniformLocations[uniformName];
	glUniform1i(location, val? 1 : 0);
}

void GlShader::UploadTexture(const char *uniformName, Texture *texture)
{
	if(this->uniformLocations.find(uniformName) == this->uniformLocations.end())
	{
		this->UploadInt(uniformName, this->textureLocationToTextureSlot.size());
		const unsigned int location = this->uniformLocations[uniformName];
		this->textureLocationToTextureSlot[location] = textureSlotsCount++;
	}

	const unsigned int location = this->uniformLocations[uniformName];
	const unsigned int textureSlot = this->textureLocationToTextureSlot[location];
	glActiveTexture(textureSlot);
	texture->Bind();
}

void GlShader::UploadUniformBuffer(const char *uniformBufferName, UniformBuffer *ubo)
{
	if(this->uniformLocations.find(uniformBufferName) == this->uniformLocations.end())
	{
		this->uniformLocations[uniformBufferName] = glGetUniformBlockIndex(this->programId, uniformBufferName);
	}

	GlUniformBuffer *buffer = (GlUniformBuffer*)ubo;
	glUniformBlockBinding(
		this->programId, 
		this->uniformLocations[uniformBufferName],
		buffer->GetBinding());
}

void GlShader::Bind()
{
    glUseProgram(this->programId);
}

void GlShader::Unbind()
{
    glUseProgram(0);
}


} // namespace mr
