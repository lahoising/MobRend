#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "mr_logger.h"
#include "mr_platform/mr_gl_shader.h"
#include "mr_texture.h"
#include "mr_asset_manager.h"

namespace mr
{
    
GlShader::GlShader(Shader::CreateParams params)
{
    // Create the shaders
	GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	/// TODO: change std string for a data structure that doesn't allocate in heap
	/// TODO: calculate diffuse in view space, not normal space
    std::string vertexShaderCode = R"(
        #version 410 core
        
        layout(location = 0) in vec3 a_pos;
        layout(location = 1) in vec3 a_normal;
		layout(location = 2) in vec2 a_texCoord;

		layout(location = 0) out vec3 out_fragPos;
		layout(location = 1) out vec3 out_normal;
		layout(location = 2) out vec2 out_texCoord;
		
		uniform mat4 u_viewProjection;
		uniform mat4 u_model;
		uniform mat3 u_normalMat;

        void main(){
			vec4 pos = u_model * vec4(a_pos, 1.0);
            gl_Position = u_viewProjection * pos;

			out_fragPos = vec3(pos);
			out_normal = u_normalMat * a_normal;
			out_texCoord = a_texCoord;
        }
    )";

    std::string fragmentShaderCode = R"(
        #version 410 core
		
		out vec4 finalFragColor;
		layout(location = 0) in vec3 a_fragPos;
		layout(location = 1) in vec3 a_normal;
		layout(location = 2) in vec2 a_texCoord;

		struct PhongMaterial
		{
			vec3 ambient;
			vec3 diffuse;
			vec3 specular;
			float shininess;
		};
        
		uniform vec3 u_color;
		uniform vec3 u_lightColor;
		uniform vec3 u_lightPos;
		uniform vec3 u_viewPos;
		uniform sampler2D u_texture;
		uniform float u_textureStrength;
		uniform PhongMaterial u_phongMaterial;
        
		void main(){
			vec3 ambient = u_phongMaterial.ambient * u_lightColor;

			vec3 norm = normalize(a_normal);
			vec3 lightDir = normalize(u_lightPos - a_fragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = (diff * u_phongMaterial.diffuse) * u_lightColor;

			vec3 viewDir = normalize(u_viewPos - a_fragPos);
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_phongMaterial.shininess);
			vec3 specular = u_phongMaterial.specular * spec * u_lightColor;

			vec3 result = (ambient + diffuse + specular) * u_color;
			vec4 textureColor = mix(
				vec4(1.0, 1.0, 1.0, 1.0),
				texture(u_texture, a_texCoord),
				u_textureStrength);
			finalFragColor = vec4(result, 1.0) * textureColor;
        }
    )";

	AssetManager &assetManager = AssetManager::GetInstance();
	if(params.vertFilePath)
	{
		auto buffer = assetManager.GetFileContent(params.vertFilePath);
		vertexShaderCode.assign(buffer.begin(), buffer.end());
	}

	if(params.fragFilePath)
	{
		auto buffer = assetManager.GetFileContent(params.fragFilePath);
		fragmentShaderCode.assign(buffer.begin(), buffer.end());
	}

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

void GlShader::UploadMat4(const char *uniformName, glm::mat4 matrix)
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

void GlShader::UploadMat3(const char *uniformName, glm::mat3 matrix)
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

void GlShader::UploadVec3(const char *uniformName, glm::vec3 vec)
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

void GlShader::UploadInt(const char *uniformName, int i)
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

void GlShader::UploadTexture2D(const char *uniformName, Texture *texture)
{
	
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
