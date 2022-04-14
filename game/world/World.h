#pragma once

#include <unordered_map>
#include <unordered_set>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include "render/Shader.h"

#include "Chunk.h"
#include "ChunkGenerator.h"
#include "Renderer.h"

class World
{
public:
    inline static auto &Instance()
    {
        static World instance;
        return instance;
    }

    static void Initialize();
    static void Finalize();

    static unsigned char GetBlock(const glm::ivec3 &worldPos);
    static void SetBlock(const glm::ivec3 &worldPos, unsigned char blockID);
    static glm::ivec3 GetChunkCoord(const glm::ivec3 &worldPos);
    static glm::ivec3 GetChunkIndex(const glm::ivec3 &worldPos);

    static void SetShader(const Shader &shader) { Instance().m_Shader = shader; }
    static void SetSeed(int seed) { ChunkGenerator::SetSeed(seed); }
    static void ProcessRequestedChunks(int renderDistance, const glm::ivec3 &centerChunkIndex);

    // Shader Functions
    const Shader &GetShader() { return m_Shader; }
    static void WorldThread();

private:
    World() {}
    World(World const &);
    void operator=(World const &) {}

    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be
    // implemented.
    std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ChunkDataMap;
    std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ChunkRenderMap;

    std::unordered_set<glm::ivec3> m_ChunksToDelete;
    std::unordered_set<glm::ivec3> m_ChunksToGenerate;

    std::unordered_set<glm::ivec3> m_ChunksToRender;
    std::unordered_set<glm::ivec3> m_ChunksToUnrender;

    Shader m_Shader;
    int m_Seed;

    std::thread m_Thread;
};