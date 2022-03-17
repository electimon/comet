#pragma once

#include "render/containers/Mesh.h"
#include "render/containers/Shader.h"

class Renderer
{
public:
    Renderer(const Renderer&) = delete;
    static void Create()
    {
        static Renderer s_Instance;
    }
    static Renderer &Get()
    {
        static Renderer s_Instance;
        return s_Instance;
    }

    static void NewFrame();
    static void SwapBuffers();
    static void DrawMeshQueue() { Get().DrawMeshQueueFunction(); }

    void AddToMeshQueue(Mesh *mesh);

private:
    Renderer();
    ~Renderer();

    void DrawMeshQueueFunction();

    std::vector<Mesh *> m_MeshQueue;
};