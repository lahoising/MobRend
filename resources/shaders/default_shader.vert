#version 430 core
        
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

layout(location = 0) out vec3 out_fragPos;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec2 out_texCoord;

/// TODO: use UBOs
// layout(set = 0, binding = 0) uniform CameraUniforms
// {
//     mat4 viewProjection;
//     mat4 model;
//     mat3 normalMat;
// } u_cam;

layout(push_constant,std430) uniform CameraUniforms
{
    mat4 viewProjection;
    mat4 model;
    mat3 normalMat;
} u_cam;

void main(){
    vec4 pos = u_cam.model * vec4(a_pos, 1.0);
    gl_Position = u_cam.viewProjection * pos;

    out_fragPos = vec3(pos);
    out_normal = u_cam.normalMat * a_normal;
    out_texCoord = a_texCoord;
}