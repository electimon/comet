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

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::NewFrame()
{
    // Clearing the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setting the new frame color to be black
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(WindowHandler::GetInstance().GetGLFWWindow());
}

void Renderer::DrawMeshesFunction()
{
    for (auto mesh : m_MeshMap)
    {
        mesh.second->Bind();

        glUniformMatrix4fv(glGetUniformLocation(mesh.second->GetShaderID(), "u_ViewMatrix"), 1, GL_FALSE, &Camera::GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(mesh.second->GetShaderID(), "u_ProjMatrix"), 1, GL_FALSE, &Camera::GetProjMatrix()[0][0]);

        glDrawElements(GL_TRIANGLES, mesh.second->GetCount(), GL_UNSIGNED_INT, (void *)0);
    }
}