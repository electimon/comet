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

void Renderer::DrawMeshesFunction()
{
    UpdateMeshQueue();

    for (auto &mesh : m_MeshMap)
    {
        // Binding the texture map
        glBindTexture(GL_TEXTURE_2D, 1);

        // Binding the next mesh in queue
        mesh.second.Bind();

        // Uniforms
        unsigned int shaderID = mesh.second.GetShaderID();
        if (mesh.second.GetTimeDelta() < 1.0)
        {
            mesh.second.Update();
            mesh.second.m_ModelMatrix = glm::translate(
                glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -50.0f, 0.0f)),
                glm::vec3(0.0f, static_cast<float>(50.0 * glm::sin(3.141592653589 / 2 * mesh.second.GetTimeDelta())), 0.0f));

            // glUniform1f(glGetUniformLocation(shaderID, "u_Brightness"), 1.0f);
            glUniform1f(glGetUniformLocation(shaderID, "u_Brightness"), mesh.second.GetBrightness());
        }
        else
        {
            mesh.second.m_ModelMatrix = glm::mat4(1.0f);
            glUniform1f(glGetUniformLocation(shaderID, "u_Brightness"), 1.0f);
        }
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ModelMatrix"), 1, GL_FALSE, &mesh.second.m_ModelMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ViewMatrix"), 1, GL_FALSE, &Camera::GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ProjMatrix"), 1, GL_FALSE, &Camera::GetProjMatrix()[0][0]);
        glUniform1f(glGetUniformLocation(shaderID, "u_Time"), glfwGetTime());
        glUniform1i(glGetUniformLocation(shaderID, "u_TextureMap"), 0);

        // Drawing mesh
        glDrawElements(GL_TRIANGLES, mesh.second.GetCount(), GL_UNSIGNED_INT, (void *)0);
    }
}

void Renderer::UpdateMeshQueue()
{
    std::lock_guard<std::mutex> locked(GetInstance().SafeToModifyMeshQueue);

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