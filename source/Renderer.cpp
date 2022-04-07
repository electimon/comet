#include "Renderer.h"

void Renderer::Initialize() {
  // Enables culling of the back faces
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Enables depth testing
  glEnable(GL_DEPTH_TEST);

  // Enables transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::NewFrame() {
  // Switch between triangle rendering modes with keys L and N
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_L) == GLFW_PRESS) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_N) == GLFW_PRESS) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  // Clearing the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Background color
  glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 0.0f);
}

void Renderer::SwapBuffers() {
  glfwSwapBuffers(WindowHandler::GetInstance().GetGLFWWindow());
}

void Renderer::DrawMeshQueue() {
  unsigned int shaderID;

  ProcessMeshQueues();

  // Binding the texture map
  glBindTexture(GL_TEXTURE_2D, 1);

  for (auto &mesh : GetInstance().m_MeshMap) {
    shaderID = mesh.second.GetShader()->GetID();

    mesh.second.Update();

    // Binding the next mesh in queue
    mesh.second.Bind();

    // Uniforms
    glUniform3iv(glGetUniformLocation(shaderID, "u_Index"), 1, &mesh.first[0]);
    glUniform1f(glGetUniformLocation(shaderID, "u_Transparency"),
                mesh.second.GetTransparency());
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ModelMatrix"), 1,
                       GL_FALSE, &mesh.second.GetModelMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ViewMatrix"), 1,
                       GL_FALSE, &Camera::GetViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "u_ProjMatrix"), 1,
                       GL_FALSE, &Camera::GetProjMatrix()[0][0]);
    glUniform1i(glGetUniformLocation(shaderID, "u_Texture"), 0);

    // Drawing mesh
    glDrawElements(GL_TRIANGLES, mesh.second.GetCount(), GL_UNSIGNED_INT,
                   (void *)0);
  }
}

void Renderer::ProcessMeshQueues() {
  {
    std::lock_guard<std::mutex> locked(GetInstance().m_AddMeshQueueLock);
    for (auto &mesh : GetInstance().m_MeshesToAdd) {
      GetInstance().m_MeshMap.insert_or_assign(mesh.first, mesh.second);
      GetInstance().m_MeshMap.at(mesh.first).Initialize();
    }
    GetInstance().m_MeshesToAdd.clear();
  }

  {
    std::lock_guard<std::mutex> locked(GetInstance().m_UpdateMeshQueueLock);
    for (auto &mesh : GetInstance().m_MeshesToUpdate) {
      GetInstance().m_MeshMap.at(mesh).UpdateGeometry();
    }
    GetInstance().m_MeshesToUpdate.clear();
  }

  {
    std::lock_guard<std::mutex> locked(GetInstance().m_DeleteMeshQueueLock);
    for (auto &mesh : GetInstance().m_MeshesToDelete) {
      GetInstance().m_MeshMap.at(mesh).Finalize();
      GetInstance().m_MeshMap.erase(mesh);
    }
    GetInstance().m_MeshesToDelete.clear();
  }
}

void Renderer::AddMeshToQueue(const glm::ivec3 &index, const Mesh &mesh) {
  std::lock_guard<std::mutex> locked(GetInstance().m_AddMeshQueueLock);
  GetInstance().m_MeshesToAdd.insert_or_assign(index, mesh);
}

void Renderer::UpdateMeshInQueue(const glm::ivec3 &index) {
  std::lock_guard<std::mutex> locked(GetInstance().m_UpdateMeshQueueLock);
  GetInstance().m_MeshesToUpdate.insert(index);
}

void Renderer::DeleteMeshFromQueue(const glm::ivec3 &index) {
  std::lock_guard<std::mutex> locked(GetInstance().m_DeleteMeshQueueLock);
  GetInstance().m_MeshesToDelete.insert(index);
}