#version 430 core

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 a_texCoord;

layout(binding = 0) uniform sampler2D u_tex;

const float offset = 1.0 / 300.0;
void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2( 0.0f, offset), // top-center
        vec2( offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2( 0.0f, 0.0f), // center-center
        vec2( offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f, -offset), // bottom-center
        vec2( offset, -offset) // bottom-right
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        vec3 sampledColor = vec3(texture(u_tex, a_texCoord + offsets[i]));
        col += sampledColor * kernel[i];
    }

    out_color = vec4(col, 1.0);
}