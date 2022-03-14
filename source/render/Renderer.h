#pragma once

#include "Engine.h"

#include "render/containers/Mesh.h"
#include "render/containers/Shader.h"

class Renderer
{
public:
    Renderer(Engine *engine);
    ~Renderer();

    void NewFrame();
    void EndFrame();

    void AddToMeshQueue(Mesh *mesh);
    void DrawMeshQueue();

    // TODO: add functions to add stuff to rendering queue

private:
    Engine *p_Engine;

    std::vector<Mesh *> m_MeshQueue;
};