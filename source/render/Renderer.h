#pragma once

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
    static void AddMesh(glm::ivec3 index, Mesh *mesh)
    {
        GetInstance().m_MeshMap.insert_or_assign(index, mesh);
    }
    static Mesh *GetMesh(glm::ivec3 index)
    {
        return GetInstance().m_MeshMap.at(index);
    }

    static void RemoveMesh(glm::ivec3 index)
    {
        if (GetInstance().m_MeshMap.find(index) != GetInstance().m_MeshMap.end())
        {
            // GetInstance().m_MeshMap.at(index)->~Mesh();
            delete GetInstance().m_MeshMap.at(index);

            GetInstance().m_MeshMap.erase(index);
        }
        else
        {
            std::cout << "Attmpted to remove mesh from queue that did not exist." << std::endl;
        }
    }

private:
    Renderer() {}
    Renderer(Renderer const &);
    void operator=(Renderer const &);

    void DrawMeshesFunction();

    // std::vector<Mesh *> m_MeshQueue;
    std::unordered_map<glm::ivec3, Mesh *> m_MeshMap;
};