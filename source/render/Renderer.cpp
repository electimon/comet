#include "Renderer.h"

Renderer::Renderer(Engine *engine)
    : p_Engine(engine)
{
}

Renderer::~Renderer()
{
}

void Renderer::NewFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
}

void Renderer::EndFrame()
{
    glfwSwapBuffers(glfwGetCurrentContext());
}