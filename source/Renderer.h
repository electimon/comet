#pragma once

#include <comet.pch>

#include "handlers/WindowHandler.h"

#include "render/Mesh.h"
#include "render/ShaderProgram.h"

#include "Camera.h"

class Renderer
{
public:
  inline static auto &Instance()
  {
    static Renderer instance;
    return instance;
  }

  static void Initialize();
  static void NewFrame();
  static void SwapBuffers();

  static void DrawMeshQueue();

  static void ProcessMeshQueues();
  static void AddMeshToQueue(const glm::ivec3 &index, const Mesh &mesh);
  static void UpdateMeshInQueue(const glm::ivec3 &index);
  static void DeleteMeshFromQueue(const glm::ivec3 &index);

private:
  Renderer() {}
  Renderer(Renderer const &);
  void operator=(Renderer const &);

  std::unordered_map<glm::ivec3, Mesh> m_MeshMap;

  // Queues for safe mesh management
  std::unordered_map<glm::ivec3, Mesh> m_MeshesToAdd;
  std::unordered_set<glm::ivec3> m_MeshesToUpdate;
  std::unordered_set<glm::ivec3> m_MeshesToDelete;
  std::mutex m_AddMeshQueueLock;
  std::mutex m_UpdateMeshQueueLock;
  std::mutex m_DeleteMeshQueueLock;
};