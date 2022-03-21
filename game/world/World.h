#pragma once

#include <unordered_map>

#include "glm/gtx/hash.hpp"

#include "render/Renderer.h"

#include "Chunk.h"

/*
index = used to describe chunk locations
coordinate = used to describe block locations
*/

class World
{
public:
    World()
    {
        std::cout << "World::World()" << std::endl;
    }
    ~World()
    {
        std::cout << "World::~World()" << std::endl;
    }

    void GenerateChunk(glm::ivec3 index)
    {
        Chunk *chunk = new Chunk(index);
        m_Chunks.insert_or_assign(index, chunk);
    }
    void GenerateChunk(int x, int y, int z) { GenerateChunk(glm::ivec3(x, y, z)); }

    void AddChunkToRenderQueue(glm::ivec3 index)
    {
        Renderer::AddMesh(index, m_Chunks.at(index)->GetMesh());
    }
    void AddChunkToRenderQueue(int x, int y, int z) { AddChunkToRenderQueue(glm::ivec3(x, y, z)); }

    void AddShaderToChunk(glm::ivec3 index, unsigned int shader)
    {
        m_Chunks.at(index)->GetMesh()->SetShaderID(shader);
    }
    void AddShaderToChunk(int x, int y, int z, unsigned int shader) { AddShaderToChunk(glm::ivec3(x, y, z), shader); }

    // void RedrawChunk(glm::ivec3 index)
    // {
    //     m_Chunks.at(index)->Redraw();
    // }

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk *> m_Chunks;
};