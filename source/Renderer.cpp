#include "Renderer.h"

Lock Renderer::QueueLock;

void Renderer::Initialize()
{
    // Creates the static instance in memory
    Instance();

    glEnable(GL_DEPTH_TEST);

    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    // (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(WindowHandler::Window(), true);
    ImGui_ImplOpenGL3_Init("#version 450");

    Instance().m_BackgroundColor = glm::vec3(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f);
}

void Renderer::NewFrame()
{
    // Clearing the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Background color
    glClearColor(Instance().m_BackgroundColor.x, Instance().m_BackgroundColor.y, Instance().m_BackgroundColor.z, 0.0f);
}

void Renderer::SwapBuffers() { glfwSwapBuffers(glfwGetCurrentContext()); }

void Renderer::ResetRenderer()
{
    SetResetting(true);

    // Resetting all render queues
    Instance().m_SolidMeshMap.clear();
    Instance().m_TransparentMeshMap.clear();
    Instance().m_MeshesToAdd.clear();
    Instance().m_MeshesToDelete.clear();
    Instance().m_MeshesToUpdate.clear();
}

void Renderer::DrawMeshQueue()
{
    unsigned int drawCalls = 0;

    unsigned int shaderID;
    auto &renderer = Instance();

    ProcessMeshQueues();

    // Binding the texture map
    glBindTexture(GL_TEXTURE_2D, 1);

    // Solid Geometry Loop
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    for (auto &[index, mesh] : Instance().m_SolidMeshMap)
    {
        shaderID = mesh.Shader()->GetID();

        mesh.Update();

        // Binding the next mesh in queue
        mesh.Bind();

        // Uniforms
        glUniform3iv(glGetUniformLocation(shaderID, "u_Index"), 1, &index[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "u_OverlayColor"), 1, &OverlayColor()[0]);
        glUniform1f(glGetUniformLocation(shaderID, "u_Transparency"), mesh.Transparency());
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ModelMatrix"), 1, GL_FALSE, &mesh.ModelMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ViewMatrix"), 1, GL_FALSE, &Camera::ViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ProjMatrix"), 1, GL_FALSE, &Camera::ProjMatrix()[0][0]);
        glUniform1i(glGetUniformLocation(shaderID, "u_Texture"), 0);

        // Drawing mesh
        glDrawElements(GL_TRIANGLES, mesh.Count(), GL_UNSIGNED_INT, (void *)0);
        drawCalls++;
    }

    // Transparent Geometry Loop
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    for (auto &[index, mesh] : Instance().m_TransparentMeshMap)
    {
        // To save some draw calls, if the mesh contains no transparency,
        // the mesh is skipped in this loop
        if (!(mesh.Count() > 0))
            continue;

        shaderID = mesh.Shader()->GetID();

        mesh.Update();

        // Binding the next mesh in queue
        mesh.Bind();

        // Uniforms
        glUniform3iv(glGetUniformLocation(shaderID, "u_Index"), 1, &index[0]);
        glUniform1f(glGetUniformLocation(shaderID, "u_Transparency"), mesh.Transparency());
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ModelMatrix"), 1, GL_FALSE, &mesh.ModelMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ViewMatrix"), 1, GL_FALSE, &Camera::ViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ProjMatrix"), 1, GL_FALSE, &Camera::ProjMatrix()[0][0]);
        glUniform1i(glGetUniformLocation(shaderID, "u_Texture"), 0);

        // Drawing mesh
        glDrawElements(GL_TRIANGLES, mesh.Count(), GL_UNSIGNED_INT, (void *)0);
        drawCalls++;
    }
    Instance().m_DrawCallsPerFrame = drawCalls;
}

void Renderer::DrawInterfaceQueue()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    InterfaceHandler::DrawInterfaces();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Renderer::AddMeshToQueue(glm::ivec3 index, const Mesh &mesh)
{
    QueueLock.AddQueue.lock();
    Instance().m_MeshesToAdd.insert_or_assign(index, mesh);
    QueueLock.AddQueue.unlock();
}

void Renderer::UpdateMeshInQueue(glm::ivec3 index)
{
    QueueLock.UpdateQueue.lock();
    Instance().m_MeshesToUpdate.insert(index);
    QueueLock.UpdateQueue.unlock();
}

void Renderer::DeleteMeshFromQueue(glm::ivec3 index)
{
    QueueLock.DeleteQueue.lock();
    Instance().m_MeshesToDelete.insert(index);
    QueueLock.DeleteQueue.unlock();
}

void Renderer::ProcessMeshQueues()
{
    // Adding meshes to the queue
    QueueLock.AddQueue.lock();
    for (const auto &[index, mesh] : Instance().m_MeshesToAdd)
    {
        if (index.y > 0)
        {
            Instance().m_TransparentMeshMap.insert_or_assign(index, mesh);
            Instance().m_TransparentMeshMap.at(index).Initialize();
        }
        else
        {
            Instance().m_SolidMeshMap.insert_or_assign(index, mesh);
            Instance().m_SolidMeshMap.at(index).Initialize();
        }
    }
    Instance().m_MeshesToAdd.clear();
    QueueLock.AddQueue.unlock();

    // Updating meshes in the queue
    QueueLock.UpdateQueue.lock();
    for (const auto &index : Instance().m_MeshesToUpdate)
    {
        if (index.y > 0)
        {
            Instance().m_TransparentMeshMap.at(index).UpdateGeometry();
        }
        else
        {
            Instance().m_SolidMeshMap.at(index).UpdateGeometry();
        }
    }
    Instance().m_MeshesToUpdate.clear();
    QueueLock.UpdateQueue.unlock();

    // Deleting meshes in the queue
    QueueLock.DeleteQueue.lock();
    for (const auto &index : Instance().m_MeshesToDelete)
    {
        if (index.y > 0)
        {
            if (Instance().m_TransparentMeshMap.find(index) != Instance().m_TransparentMeshMap.end())
            {
                Instance().m_TransparentMeshMap.at(index).Finalize();
                Instance().m_TransparentMeshMap.erase(index);
            }
        }
        else
        {
            if (Instance().m_SolidMeshMap.find(index) != Instance().m_SolidMeshMap.end())
            {
                Instance().m_SolidMeshMap.at(index).Finalize();
                Instance().m_SolidMeshMap.erase(index);
            }
        }
    }
    Instance().m_MeshesToDelete.clear();
    QueueLock.DeleteQueue.unlock();
}
