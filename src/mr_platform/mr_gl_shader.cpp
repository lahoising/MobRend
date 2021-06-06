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
			vec3 diffuse;
			sampler2D diffuseMap;
			float diffuseMapStrength;

			vec3 specular;
			sampler2D specularMap;
			float specularMapStrength;

			float shininess;
		};

		struct Light
		{
			vec3 position;
			vec3 color;
			float intensity;
			int type;

			vec3 attenuation;
			float innerCutoff;
			float outerCutoff;
		};
        
		uniform vec3 u_color;
		uniform vec3 u_viewPos;
		uniform PhongMaterial u_phongMaterial;
		
		uniform Light u_pointLight;
		uniform Light u_ambientLight;
        
		vec3 AmbientLight(Light light)
		{
			return light.color * light.intensity * float(light.type == 0);
		}

		vec3 DiffuseLight(Light light, vec3 normal, vec3 fragmentPosition)
		{
			vec3 lightDir = normalize(light.position - fragmentPosition) * float(light.type == 1 || light.type == 3);
			lightDir += normalize(-light.position) * float(light.type == 2); // position is direction if light is directional
			float diff = max(dot(normal, lightDir), 0.0);
			return light.color * light.intensity * diff;
		}

		vec3 SpecularLight(Light light, vec3 viewDir, vec3 fragPosition, vec3 normal, float shininess)
		{
			vec3 lightDir = normalize(light.position - fragPosition) * float(light.type == 1 || light.type == 3);
			lightDir += normalize(-light.position) * float(light.type == 2);
			vec3 reflectDir = reflect(-lightDir, normal);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
			return vec3(1.0f, 1.0f, 1.0f) * light.intensity * spec;
		}

		float Attenuation(Light light, vec3 fragPosition)
		{
			float distance = length(light.position - fragPosition);
			float denominator = light.attenuation.x + 
								(light.attenuation.y * distance) + 
								(light.attenuation.z * distance * distance);
			return mix(1.0, 1.0 / denominator, light.type == 1); // dont use attenuation when light is not point
		}

		float WithinSpotlight(Light light, vec3 fragPosition)
		{
			vec3 lightDir = normalize(light.position - fragPosition);
			float theta = dot(lightDir, normalize(-light.attenuation));
			float epsilon = light.innerCutoff - light.outerCutoff;
			float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
			return mix(1.0, intensity, light.type == 3);
		}

		void main(){
			vec3 diffuseColor = mix(
				u_phongMaterial.diffuse,
				vec3(texture(u_phongMaterial.diffuseMap, a_texCoord)),
				u_phongMaterial.diffuseMapStrength);

			vec3 specularColor = mix(
				u_phongMaterial.specular,
				vec3(texture(u_phongMaterial.specularMap, a_texCoord)),
				u_phongMaterial.specularMapStrength);

			float pointLightAttenuation = Attenuation(u_pointLight, a_fragPos);
			float ambientAttenuation = Attenuation(u_ambientLight, a_fragPos);

			float ambientSpotlight = WithinSpotlight(u_ambientLight, a_fragPos);

			vec3 ambientLight = AmbientLight(u_ambientLight) * ambientAttenuation * ambientSpotlight+ 
								AmbientLight(u_pointLight) * pointLightAttenuation;
			vec3 ambient = diffuseColor * ambientLight;

			vec3 norm = normalize(a_normal);
			vec3 diffuseLight = DiffuseLight(u_ambientLight, norm, a_fragPos) * ambientAttenuation * ambientSpotlight + 
								DiffuseLight(u_pointLight, norm, a_fragPos) * pointLightAttenuation;
			vec3 diffuse = diffuseColor * diffuseLight;

			vec3 viewDir = normalize(u_viewPos - a_fragPos);
			vec3 specLight = 	SpecularLight(u_ambientLight, viewDir, a_fragPos, norm, u_phongMaterial.shininess) * ambientAttenuation * ambientSpotlight +
								SpecularLight(u_pointLight, viewDir, a_fragPos, norm, u_phongMaterial.shininess) * pointLightAttenuation;
			vec3 specular = specularColor * specLight;

			vec3 result = (ambient + diffuse + specular) * u_color;
			finalFragColor = vec4(result, 1.0);
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
