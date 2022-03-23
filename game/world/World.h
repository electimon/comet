#pragma once

#include <unordered_map>
#include <unordered_set>

#include "glm/glm.hpp"

#include "glm/gtx/hash.hpp"

#include "Renderer.h"

#include "Chunk.h"

class World
{
public:
    World();
    ~World();

    static int GetChunkSize() { return 32; }
    static int GetChunkHeight() { return 64; }
    static int GetWaterHeight() { return 8; }

    void GenerateChunk(const glm::ivec3 &index);
    void GenerateChunk(int x, int y, int z);

    void AddChunkToRenderQueue(const glm::ivec3 &index);
    void AddChunkToRenderQueue(int x, int y, int z);

    void AddShaderToChunk(const glm::ivec3 &index, unsigned int shader);
    void AddShaderToChunk(int x, int y, int z, unsigned int shader);

    void ProcessRequestedChunks(const std::unordered_set<glm::ivec3> &chunks);

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk *> m_ChunkMap;
};