#include <mr_window.h>
#include <mr_buffer.h>
#include <mr_vertex_buffer.h>
#include <mr_index_buffer.h>
#include <mr_framebuffer.h>
#include <mr_renderer.h>
#include <mr_uniform_buffer.h>
#include <mr_texture.h>
#include <mr_shader.h>
#include <mr_light.h>

namespace mr
{
    Window::~Window() {}
    Buffer::~Buffer(){}
    VertexBuffer::~VertexBuffer(){}
    IndexBuffer::~IndexBuffer(){}
    UniformBuffer::~UniformBuffer(){}
    Framebuffer::~Framebuffer(){}
    Renderer::~Renderer(){}
    Texture::~Texture(){}
    Shader::~Shader(){}
    Light::~Light(){}
} // namespace mr
