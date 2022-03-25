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

    static int GetChunkSize() { return 16; }
    static int GetChunkHeight() { return 512; }
    static int GetWaterHeight() { return 16; }

    void GenerateChunk(const glm::ivec3 &index);

    void ProcessRequestedChunks(const std::unordered_set<glm::ivec3> &chunks);

    unsigned int GetShader() { return m_Shader; }
    void SetShader(unsigned int shader) { m_Shader = shader; }

    void EndWorldThread() { m_EndThread = true; }

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk *> m_ChunkDataMap;

    std::unordered_set<glm::ivec3> m_ChunksToDelete;
    std::unordered_set<glm::ivec3> m_ChunksToCreate;

    unsigned int m_Shader;

    std::thread m_Thread;
    void WorldThread();
    bool m_EndThread;
};