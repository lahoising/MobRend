#version 430 core
        
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

layout(location = 0) out vec3 out_fragPos;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec2 out_texCoord;

layout(binding = 0,std140) uniform CameraMatrices
{
    mat4 u_viewProjection;
    mat3 u_normalMat;
};

layout(push_constant,std140) uniform Model
{
    mat4 model;
} u_model;

void main(){
    vec4 pos = u_model.model * vec4(a_pos, 1.0);
    gl_Position = u_viewProjection * pos;

    out_fragPos = vec3(pos);
    out_normal = u_normalMat * a_normal;
    out_texCoord = a_texCoord;
}