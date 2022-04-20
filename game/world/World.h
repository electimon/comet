#pragma once

#include <comet.pch>

#include "render/ShaderProgram.h"

#include "BlockLibrary.h"
#include "Chunk.h"
#include "ChunkGenerator.h"
#include "Engine.h"
#include "Renderer.h"
#include "WorldConfig.h"

class World
{
  public:
    inline static auto &Instance()
    {
        static World instance;
        return instance;
    }

    static void Initialize();
    static void Thread();
    static void Finalize();

    static Block GetBlock(const glm::ivec3 &worldPos);
    static void SetBlock(const glm::ivec3 &worldPos, Block block);
    static glm::ivec3 GetChunkCoord(const glm::ivec3 &worldPos);
    static glm::ivec3 GetChunkIndex(const glm::ivec3 &worldPos);

    static void SetShader(const ShaderProgram &shader) { Instance().m_Shader = shader; }
    static void SetSeed(int seed) { ChunkGenerator::SetSeed(seed); }
    static int Seed() { return Instance().m_Seed; }
    static void ProcessRequestedChunks(int renderDistance, const glm::ivec3 &centerChunkIndex);

    // Shader Functions
    const ShaderProgram &GetShader() { return m_Shader; }
    static void WorldThread();

  private:
    World() {}
    World(World const &);
    void operator=(World const &) {}

    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be
    // implemented.
    std::unordered_map<glm::ivec3, Chunk> m_ChunkDataMap;
    std::unordered_map<glm::ivec3, Chunk *> m_ChunkRenderMap;

    std::unordered_set<glm::ivec3> m_ChunksToDelete;
    std::unordered_set<glm::ivec3> m_ChunksToGenerate;

    std::unordered_set<glm::ivec3> m_ChunksToRender;
    std::unordered_set<glm::ivec3> m_ChunksToUnrender;

    ShaderProgram m_Shader;
    int m_Seed;

    std::mutex m_Lock;

    std::thread m_Thread;
};