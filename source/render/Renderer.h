#pragma once

#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "glm/gtx/hash.hpp"

#include "render/containers/Mesh.h"
#include "render/containers/Shader.h"

class Renderer
{
public:
    static Renderer &GetInstance()
    {
        static Renderer s_Instance;
        return s_Instance;
    }

    static void Initialize();
    static void NewFrame();
    static void SwapBuffers();
    static void DrawMeshes() { GetInstance().DrawMeshesFunction(); }
    static void UpdateMeshQueue();


    static void AddMeshToQueue(const glm::ivec3 &index, const Mesh &mesh)
    {
        std::lock_guard<std::mutex> locked(GetInstance().m_MeshQueueLock);
        GetInstance().m_MeshesToAdd.insert_or_assign(index, mesh);
    }

    static void DeleteMeshFromQueue(const glm::ivec3 &index)
    {
        std::lock_guard<std::mutex> locked(GetInstance().m_MeshQueueLock);
        GetInstance().m_MeshesToDelete.insert(index);
    }


    // void ProcessRequestedMeshes(const std::unordered_set<glm::ivec3> &requestedMeshes);

private:
    Renderer() {}
    Renderer(Renderer const &);
    void operator=(Renderer const &);

    void DrawMeshesFunction();
    std::mutex m_MeshQueueLock;

    // The renderer owned map
    std::unordered_map<glm::ivec3, Mesh> m_MeshMap;

    // Queues pushes from other objects
    std::unordered_map<glm::ivec3, Mesh> m_MeshesToAdd;
    std::unordered_set<glm::ivec3> m_MeshesToDelete;
};