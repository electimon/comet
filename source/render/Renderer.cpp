#include "Renderer.h"

#include "handlers/WindowHandler.h"

#include "render/containers/Mesh.h"

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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Renderer::EndFrame()
{
    glfwSwapBuffers(p_Engine->GetWindowHandler()->GetWindow());
}

void Renderer::AddToMeshQueue(Mesh *mesh)
{
    m_MeshQueue.push_back(mesh);
}

void Renderer::DrawMeshQueue()
{
    for (unsigned int i = 0; i < m_MeshQueue.size(); i++)
    {
        m_MeshQueue[i]->Bind();

        glDrawElements(GL_TRIANGLES, m_MeshQueue[i]->GetCount(), GL_UNSIGNED_INT, (void *)0);
    }
}