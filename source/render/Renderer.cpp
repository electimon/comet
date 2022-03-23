#include "Renderer.h"

#include "handlers/WindowHandler.h"

#include "render/containers/Mesh.h"
#include "render/containers/Shader.h"
#include "render/Camera.h"

void Renderer::Initialize()
{
    // Enables culling of the back faces
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enables depth testing
    glEnable(GL_DEPTH_TEST);

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::NewFrame()
{
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_L) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_N) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Clearing the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setting the new frame color to be black
    glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 0.0f);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(WindowHandler::GetInstance().GetGLFWWindow());
}

void Renderer::DrawMeshesFunction()
{
    for (auto &mesh : m_MeshesToQueue)
    {
        m_MeshMap.insert_or_assign(mesh.first, mesh.second);
    }
    m_MeshesToQueue.clear();

    for (auto &mesh : m_MeshesToDelete)
    {
        delete m_MeshMap.at(mesh);
        m_MeshMap.erase(mesh);
    }
    m_MeshesToDelete.clear();


    for (auto &mesh : m_MeshMap)
    {
        if (!mesh.second->IsPushedToGPU())
        {
            mesh.second->PushToGPU();
        }

        mesh.second->Bind();

        glUniformMatrix4fv(glGetUniformLocation(mesh.second->GetShaderID(), "u_ViewMatrix"), 1, GL_FALSE, &Camera::GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(mesh.second->GetShaderID(), "u_ProjMatrix"), 1, GL_FALSE, &Camera::GetProjMatrix()[0][0]);

        glDrawElements(GL_TRIANGLES, mesh.second->GetCount(), GL_UNSIGNED_INT, (void *)0);
    }
}