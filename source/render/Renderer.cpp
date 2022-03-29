#include "Renderer.h"

#include "glm/gtc/constants.hpp"

#include "handlers/WindowHandler.h"

#include "render/containers/Mesh.h"
#include "render/containers/Shader.h"
#include "render/Camera.h"

#include "stb_image.h"

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
    // glm::vec4 backgroundColor = glm::vec4(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 0.0f);
    // float backgroundBrightness = std::max(std::cos((float)glfwGetTime()), 0.3f);
    // glm::vec4 result = backgroundColor * backgroundBrightness;
    // glClearColor(backgroundColor.x * result.x, backgroundColor.y * result.y, backgroundColor.z * result.z, backgroundColor.w * result.w);

    glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 0.0f);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(WindowHandler::GetInstance().GetGLFWWindow());
}

void Renderer::DrawMeshQueue()
{
    UpdateMeshQueue();
    unsigned int shaderID;

    // Binding the texture map
    glBindTexture(GL_TEXTURE_2D, 1);

    for (auto &mesh : GetInstance().m_MeshMap)
    {
        // Binding the next mesh in queue
        mesh.second.Bind();

        // Uniforms
        shaderID = mesh.second.GetShaderID();

        mesh.second.Update();
        glUniform1f(glGetUniformLocation(shaderID, "u_Transparency"), mesh.second.GetTransparency());

        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ModelMatrix"), 1, GL_FALSE, &mesh.second.m_ModelMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ViewMatrix"), 1, GL_FALSE, &Camera::GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ProjMatrix"), 1, GL_FALSE, &Camera::GetProjMatrix()[0][0]);

        glUniform1i(glGetUniformLocation(shaderID, "u_Texture"), 0);

        // Drawing mesh
        glDrawElements(GL_TRIANGLES, mesh.second.GetCount(), GL_UNSIGNED_INT, (void *)0);
    }
}

void Renderer::UpdateMeshQueue()
{
    std::lock_guard<std::mutex> locked(GetInstance().m_MeshQueueLock);

    for (auto &mesh : GetInstance().m_MeshesToAdd)
    {
        GetInstance().m_MeshMap.insert_or_assign(mesh.first, mesh.second);
        GetInstance().m_MeshMap.at(mesh.first).AllocateOnGPU();
    }
    GetInstance().m_MeshesToAdd.clear();

    for (auto &mesh : GetInstance().m_MeshesToDelete)
    {
        GetInstance().m_MeshMap.at(mesh).DeallocateOnGPU();
        GetInstance().m_MeshMap.erase(mesh);
    }
    GetInstance().m_MeshesToDelete.clear();
}

void Renderer::AddMeshToQueue(const glm::ivec3 &index, const Mesh &mesh)
{
    std::lock_guard<std::mutex> locked(GetInstance().m_MeshQueueLock);
    GetInstance().m_MeshesToAdd.insert_or_assign(index, mesh);
}

void Renderer::DeleteMeshFromQueue(const glm::ivec3 &index)
{
    std::lock_guard<std::mutex> locked(GetInstance().m_MeshQueueLock);
    GetInstance().m_MeshesToDelete.insert(index);
}