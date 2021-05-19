#ifdef MOBREND_GL_RENDERING

#include <glad/glad.h>
#include "mr_platform/mr_gl_renderer.h"

#ifdef MOBREND_GLFW_WINDOW
#include <GLFW/glfw3.h>
#endif

namespace mr
{
    
GlRenderer::GlRenderer()
{
    #ifdef MOBREND_GLFW_WINDOW
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    #endif
    glClearColor(0.8f, 0.3f, 0.2f, 1.0f);
}

GlRenderer::~GlRenderer()
{

}

void GlRenderer::OnRenderBegin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlRenderer::OnRenderEnd()
{

}


} // namespace mr


#endif