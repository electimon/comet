#include "Renderer.h"

#include "handlers/WindowHandler.h"

#include "render/containers/Mesh.h"
#include "render/containers/Shader.h"
#include "render/Camera.h"

void Renderer::NewFrame()
{
    // Clearing the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enables culling of the back faces
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Setting the new frame color to be black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(WindowHandler::GetInstance().GetGLFWWindow());
}

void Renderer::AddToMeshQueue(Mesh *mesh)
{
    m_MeshQueue.push_back(mesh);
}

void Renderer::DrawMeshQueueFunction()
{
    for (unsigned int i = 0; i < m_MeshQueue.size(); i++)
    {
        m_MeshQueue[i]->Bind();

        glUniformMatrix4fv(glGetUniformLocation(m_MeshQueue[i]->GetShaderID(), "u_ViewMatrix"), 1, GL_FALSE, &Camera::GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_MeshQueue[i]->GetShaderID(), "u_ProjMatrix"), 1, GL_FALSE, &Camera::GetProjMatrix()[0][0]);

        glDrawElements(GL_TRIANGLES, m_MeshQueue[i]->GetCount(), GL_UNSIGNED_INT, (void *)0);
    }
}