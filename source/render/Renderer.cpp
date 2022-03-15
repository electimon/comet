#include "Renderer.h"

#include "handlers/WindowHandler.h"

#include "render/containers/Mesh.h"
#include "render/containers/Shader.h"
#include "render/Camera.h"

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

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(p_Engine->GetWindowHandler()->GetGLFWWindow());
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

        glUniformMatrix4fv(glGetUniformLocation(m_MeshQueue[i]->GetShaderID(), "u_ViewMatrix"), 1, GL_FALSE, &p_Engine->GetCamera()->GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_MeshQueue[i]->GetShaderID(), "u_ProjMatrix"), 1, GL_FALSE, &p_Engine->GetCamera()->GetProjMatrix()[0][0]);

        glDrawElements(GL_TRIANGLES, m_MeshQueue[i]->GetCount(), GL_UNSIGNED_INT, (void *)0);
    }
}