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

    std::string VertexShaderCode = R"(
        #version 410 core
        
        layout(location = 0) in vec3 vertexPosition_modelspace;

        void main(){
            gl_Position.xyz = vertexPosition_modelspace;
            gl_Position.w = 1.0;
        }
    )";

    std::string FragmentShaderCode = R"(
        #version 330 core
        out vec3 color;
        void main(){
            color = vec3(1,0,0);
        }
    )";

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	mrlog("Compiling shader : %s\n", params.vertFilePath);
	this->CompileShader(vertShaderId, VertexShaderCode.c_str());
	// char const * VertexSourcePointer = VertexShaderCode.c_str();
	// glShaderSource(vertShaderId, 1, &VertexSourcePointer , NULL);
	// glCompileShader(vertShaderId);

	// Check Vertex Shader
	glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(vertShaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	mrlog("Compiling shader : %s\n", params.fragFilePath);
	this->CompileShader(fragShaderId, FragmentShaderCode.c_str());
	// char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	// glShaderSource(fragShaderId, 1, &FragmentSourcePointer , NULL);
	// glCompileShader(fragShaderId);

	// Check Fragment Shader
	glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(fragShaderId, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	mrlog("Linking program\n");
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
	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);
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
