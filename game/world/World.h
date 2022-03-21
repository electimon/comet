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
    World();
    ~World();

    void GenerateChunk(glm::ivec3 index);
    void GenerateChunk(int x, int y, int z);

    void AddChunkToRenderQueue(glm::ivec3 index);
    void AddChunkToRenderQueue(int x, int y, int z);

    void AddShaderToChunk(glm::ivec3 index, unsigned int shader);
    void AddShaderToChunk(int x, int y, int z, unsigned int shader);

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk *> m_Chunks;
};