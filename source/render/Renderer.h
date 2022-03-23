#pragma once

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

    static Mesh *GetMesh(glm::ivec3 index)
    {
        return GetInstance().m_MeshMap.at(index);
    }

    static void AddMeshToQueue(const glm::ivec3 &index, Mesh *mesh)
    {
        GetInstance().m_MeshesToQueue.insert_or_assign(index, mesh);
    }

    static void AddMeshToDelete(const glm::ivec3 &index)
    {
        GetInstance().m_MeshesToDelete.insert(index);
    }


    // void ProcessRequestedMeshes(const std::unordered_set<glm::ivec3> &requestedMeshes);

private:
    Renderer() {}
    Renderer(Renderer const &);
    void operator=(Renderer const &);

    void DrawMeshesFunction();

    std::unordered_map<glm::ivec3, Mesh *> m_MeshMap;

    std::unordered_map<glm::ivec3, Mesh *> m_MeshesToQueue;
    std::unordered_set<glm::ivec3> m_MeshesToDelete;
};