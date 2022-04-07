#pragma once

#include <unordered_map>
#include <unordered_set>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include "render/Shader.h"

#include "Chunk.h"
#include "ChunkGenerator.h"
#include "Renderer.h"

class World {
public:
  static World &GetInstance() {
    static World s_Instance;
    return s_Instance;
  }

  static void Initialize();
  static void Finalize();

  static unsigned char GetBlock(const glm::vec3 &worldPos);
  static void SetBlock(const glm::vec3 &worldPos, unsigned char blockID);
  static void SetShader(const Shader &shader) { GetInstance().m_Shader = shader; }
  static void SetSeed(int seed) { ChunkGenerator::SetSeed(seed); }
  static glm::ivec3 GetChunkCoordFromWorldCoord(const glm::vec3 &worldPos);
  static glm::ivec3 GetChunkIndexFromWorldCoord(const glm::vec3 &worldPos);
  static void ProcessRequestedChunks(const std::unordered_set<glm::ivec3> &chunks);


  void GenerateChunk(const glm::ivec3 &index);
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
  std::unordered_map<glm::ivec3, Chunk *> m_ChunkDataMap;

  std::unordered_set<glm::ivec3> m_ChunksToDelete;
  std::unordered_set<glm::ivec3> m_ChunksToCreate;

  Shader m_Shader;
  int m_Seed;

  std::thread m_Thread;
};