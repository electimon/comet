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

    static void DrawMeshQueue();
    static void UpdateMeshQueue();
    static void AddMeshToQueue(const glm::ivec3 &index, const Mesh &mesh);
    static void DeleteMeshFromQueue(const glm::ivec3 &index);

private:
    Renderer() {}
    Renderer(Renderer const &);
    void operator=(Renderer const &);

    // Queues for safe mesh management
    std::unordered_map<glm::ivec3, Mesh> m_MeshMap;
    std::unordered_map<glm::ivec3, Mesh> m_MeshesToAdd;
    std::unordered_set<glm::ivec3> m_MeshesToDelete;
    std::mutex m_MeshQueueLock;
};