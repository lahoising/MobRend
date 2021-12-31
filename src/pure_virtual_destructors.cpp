#include <mobrend/window.h>
#include <mobrend/buffer.h>
#include <mobrend/vertex_buffer.h>
#include <mobrend/index_buffer.h>
#include <mobrend/framebuffer.h>
#include <mobrend/renderer.h>
#include <mobrend/uniform_buffer.h>
#include <mobrend/texture.h>
#include <mobrend/shader.h>
#include <mobrend/light.h>

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
