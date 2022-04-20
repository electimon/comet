#pragma once

#include <comet.pch>

#include "handlers/WindowHandler.h"
#include "handlers/InterfaceHandler.h"

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
    static void DrawInterfaceQueue();

    static void ProcessMeshQueues();
    static void ResetRenderer();
    static void AddMeshToQueue(const glm::ivec3 &index, const Mesh &mesh);
    static void UpdateMeshInQueue(const glm::ivec3 &index);
    static void DeleteMeshFromQueue(const glm::ivec3 &index);

    static glm::vec3 OverlayColor() { return Instance().m_OverlayColor; }
    static void SetOverlayColor(const glm::vec3 &OverlayColor) { Instance().m_OverlayColor = OverlayColor; }

    static glm::vec3 BackgroundColor() { return Instance().m_BackgroundColor; }
    static void SetBackgroundColor(const glm::vec3 &BackgroundColor) { Instance().m_BackgroundColor = BackgroundColor; }

  private:
    Renderer() {}
    Renderer(Renderer const &);
    void operator=(Renderer const &);

    glm::vec3 m_OverlayColor;
    glm::vec3 m_BackgroundColor;

    std::unordered_map<glm::ivec3, Mesh> m_SolidMeshMap;
    std::unordered_map<glm::ivec3, Mesh> m_TransparentMeshMap;

    // Queues for safe mesh management
    std::unordered_map<glm::ivec3, Mesh> m_MeshesToAdd;
    std::unordered_set<glm::ivec3> m_MeshesToUpdate;
    std::unordered_set<glm::ivec3> m_MeshesToDelete;
    std::mutex m_AddMeshQueueLock;
    std::mutex m_UpdateMeshQueueLock;
    std::mutex m_DeleteMeshQueueLock;
};