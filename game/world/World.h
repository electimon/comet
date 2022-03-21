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

    void GenerateChunk(int x, int y, int z)
    {
        GenerateChunk(glm::ivec3(x, y, z));
    }

    void GenerateChunk(glm::ivec3 index)
    {
        Chunk *chunk = new Chunk(index);
        m_Chunks.insert_or_assign(index, chunk);
    }

    void AddChunkToRenderQueue(int x, int y, int z)
    {
        AddChunkToRenderQueue(glm::ivec3(x, y, z));
    }

    void AddChunkToRenderQueue(glm::ivec3 index)
    {
        Renderer::AddToMeshQueue(m_Chunks.at(index)->GetMesh());
    }

    void AddShaderToChunk(glm::ivec3 index, Shader shader)
    {
        m_Chunks.at(index)->GetMesh()->SetShaderID(shader.GetID());
    }
    void AddShaderToChunk(int x, int y, int z, Shader &shader)
    {
        m_Chunks.at(glm::ivec3(x, y, z))->GetMesh()->SetShaderID(shader.GetID());
    }

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk *> m_Chunks;
};