#include "World.h"

#include "BlockLibrary.h"
#include "ChunkGenerator.h"
#include "Engine.h"

#include <filesystem>
#include <thread>

void World::Initialize() {
  std::cout << "World::World()" << std::endl;
  std::filesystem::create_directory("world");
  GetInstance().m_Thread = std::thread(&World::WorldThread);

  ChunkGenerator::Initialize();
  BlockLibrary::Initialize();
}

void World::Finalize() {
  std::cout << "Saving currently loaded chunks..." << std::endl;

  for (auto &chunk : GetInstance().m_ChunkDataMap) {
    chunk.second->~Chunk();
  }

  GetInstance().m_Thread.join();

  std::cout << "World::~World()" << std::endl;
}

unsigned char World::GetBlock(const glm::vec3 &worldPos) {
  glm::ivec3 index = GetChunkIndexFromWorldCoord(worldPos);
  glm::ivec3 chunkCoord = GetChunkCoordFromWorldCoord(worldPos);

  if (GetInstance().m_ChunkDataMap.find(index) !=
      GetInstance().m_ChunkDataMap.end()) {
    return GetInstance().m_ChunkDataMap.at(index)->GetBlock(chunkCoord);
  } else {
    return 0;
  }
}

void World::SetBlock(const glm::vec3 &worldPos, unsigned char blockID) {
  if (worldPos.y > CHUNK_HEIGHT) {
    return;
  }

  glm::ivec3 index = GetChunkIndexFromWorldCoord(worldPos);
  glm::ivec3 chunkCoord = GetChunkCoordFromWorldCoord(worldPos);

  if (GetInstance().m_ChunkDataMap.find(index) !=
      GetInstance().m_ChunkDataMap.end()) {
    Chunk *chunk = GetInstance().m_ChunkDataMap.at(index);

    chunk->SetBlock(chunkCoord.x, chunkCoord.y, chunkCoord.z, blockID);
    chunk->GenerateMesh();

    Renderer::UpdateMeshInQueue(index);

    return;
  } else {
    return;
  }
}

glm::ivec3 World::GetChunkCoordFromWorldCoord(const glm::vec3 &worldPos) {
  glm::ivec3 chunkIndex = GetChunkIndexFromWorldCoord(worldPos);
  glm::ivec3 chunkPos(0, 0, 0);

  chunkPos.x = static_cast<int>(worldPos.x + 0.5f) - CHUNK_WIDTH * chunkIndex.x;
  chunkPos.y = static_cast<int>(worldPos.y + 0.5f);
  chunkPos.z = static_cast<int>(worldPos.z + 0.5f) - CHUNK_WIDTH * chunkIndex.z;

  if (chunkIndex.x < 0)
    chunkPos.x -= 1;
  if (chunkIndex.z < 0)
    chunkPos.z -= 1;

  return chunkPos;
}

glm::ivec3 World::GetChunkIndexFromWorldCoord(const glm::vec3 &worldPos) {
  glm::ivec3 chunkIndex(0, 0, 0);

  if (worldPos.x + 0.5f < 0.0f) {
    chunkIndex.x = (worldPos.x - CHUNK_WIDTH) / CHUNK_WIDTH;
  } else {
    chunkIndex.x = worldPos.x / CHUNK_WIDTH;
  }

  if (worldPos.z + 0.5f < 0.0f) {
    chunkIndex.z = (worldPos.z - CHUNK_WIDTH) / CHUNK_WIDTH;
  } else {
    chunkIndex.z = worldPos.z / CHUNK_WIDTH;
  }

  return chunkIndex;
}

void World::GenerateChunk(const glm::ivec3 &index) {
  Chunk *chunk = new Chunk(index);
  m_ChunkDataMap.insert_or_assign(index, chunk);
}

void World::ProcessRequestedChunks(
    const std::unordered_set<glm::ivec3> &requestedChunks) {
  // Loops through the currently loaded chunks and checks if they are
  // present in the requested chunks. If not, added to the chunksToDelete set.
  // Removes from the rendering mesh queue as well. Removing from the map
  // while looping over it is bad.
  for (const std::pair<const glm::ivec3, Chunk *> &oldChunk :
       GetInstance().m_ChunkDataMap) {
    if (requestedChunks.find(oldChunk.first) == requestedChunks.end()) {
      GetInstance().m_ChunksToDelete.insert(oldChunk.first);
    }
  }

  for (const glm::ivec3 &newChunk : requestedChunks) {
    if (GetInstance().m_ChunkDataMap.find(newChunk) ==
        GetInstance().m_ChunkDataMap.end()) {
      GetInstance().m_ChunksToCreate.insert(newChunk);
    }
  }

  // Perform full loop through chunks if there is a desync with loaded chunks
  if (GetInstance().m_ChunkDataMap.size() != requestedChunks.size()) {
    for (auto &chunk : GetInstance().m_ChunkDataMap) {
      if (requestedChunks.find(chunk.first) == requestedChunks.end()) {
        GetInstance().m_ChunksToDelete.insert(chunk.first);
      }
    }
  }
}

void World::WorldThread() {
  while (!Engine::ShouldClose()) {
    GetInstance().m_ChunkDataMap.reserve(GetInstance().m_ChunksToCreate.size());

    // Create new chunks
    for (const glm::ivec3 &index : GetInstance().m_ChunksToCreate) {
      // add chunk to data
      Chunk *chunk = new Chunk(index); // heap allocation
      GetInstance().m_ChunkDataMap.insert_or_assign(index, chunk);

      // add mesh to renderer
      Mesh mesh = Mesh(GetInstance().m_ChunkDataMap.at(index)->GetVertices(),
                       GetInstance().m_ChunkDataMap.at(index)->GetIndices(),
                       &GetInstance().m_Shader);
      Renderer::AddMeshToQueue(index, mesh);
    }
    GetInstance().m_ChunksToCreate.clear();

    // Delete old chunks
    for (const glm::ivec3 &index : GetInstance().m_ChunksToDelete) {
      // remove chunk from data
      delete GetInstance().m_ChunkDataMap.at(index); // heap deletion
      GetInstance().m_ChunkDataMap.erase(index);

      // remove mesh from renderer
      Renderer::DeleteMeshFromQueue(index);
    }
    GetInstance().m_ChunksToDelete.clear();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}