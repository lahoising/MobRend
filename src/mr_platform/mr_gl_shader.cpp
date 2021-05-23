#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include "mr_logger.h"
#include "mr_platform/mr_gl_shader.h"

namespace mr
{
    
GlShader::GlShader(Shader::CreateParams params)
{
    // Create the shaders
	GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    std::string defaultVertexShaderCode = R"(
        #version 410 core
        
        layout(location = 0) in vec3 vertexPosition_modelspace;

        void main(){
            gl_Position.xyz = vertexPosition_modelspace;
            gl_Position.w = 1.0;
        }
    )";

    std::string defaultFragmentShaderCode = R"(
        #version 330 core
        out vec3 color;
        void main(){
            color = vec3(1,0,0);
        }
    )";

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	mrlog("Compiling shader : %s", params.vertFilePath);
	this->CompileShader(vertShaderId, defaultVertexShaderCode.c_str());

	// Compile Fragment Shader
	mrlog("Compiling shader : %s", params.fragFilePath);
	this->CompileShader(fragShaderId, defaultFragmentShaderCode.c_str());

	// Link the program
	mrlog("Linking program");
    this->programId = glCreateProgram();
	glAttachShader(this->programId, vertShaderId);
	glAttachShader(this->programId, fragShaderId);
	glLinkProgram(this->programId);

	// Check the program
	glGetProgramiv(this->programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(this->programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
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

void GlShader::Bind()
{
    glUseProgram(this->programId);
}

void GlShader::Unbind()
{
    glUseProgram(0);
}


} // namespace mr
