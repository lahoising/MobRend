#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "mr_logger.h"
#include "mr_platform/mr_gl_shader.h"
#include "mr_texture.h"

namespace mr
{
    
GlShader::GlShader(Shader::CreateParams params)
{
    // Create the shaders
	GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    std::string defaultVertexShaderCode = R"(
        #version 410 core
        
        layout(location = 0) in vec3 a_pos;
		layout(location = 1) in vec3 a_col;
		layout(location = 2) in vec2 a_texCoord;

		layout(location = 0) out vec3 out_color;
		layout(location = 1) out vec2 out_texCoord;
		
		uniform mat4 u_viewProjection;
		uniform mat4 u_model;
		uniform vec3 u_color;

        void main(){
            gl_Position = u_viewProjection * u_model * vec4(a_pos, 1.0);
			out_color = a_col;
			out_texCoord = a_texCoord;
        }
    )";

    std::string defaultFragmentShaderCode = R"(
        #version 410 core
		
		out vec4 finalFragColor;
		layout(location = 0) in vec3 a_col;
		layout(location = 1) in vec2 a_texCoord;
        
		uniform vec3 u_color;
		uniform sampler2D u_texture;
		uniform sampler2D u_tex2;
        
		void main(){
			vec4 textureMix = mix(
				texture(u_texture, a_texCoord),
				texture(u_tex2, a_texCoord),
				0.5
			);
            finalFragColor = vec4(a_col * u_color, 1.0f) * textureMix;
        }
    )";

	this->CompileShader(vertShaderId, defaultVertexShaderCode.c_str());
	this->CompileShader(fragShaderId, defaultFragmentShaderCode.c_str());

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
