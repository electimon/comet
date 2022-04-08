#include "World.h"

#include "BlockLibrary.h"
#include "ChunkGenerator.h"
#include "Engine.h"
#include "world/WorldConfig.h"

#include <filesystem>
#include <thread>

void World::Initialize() {
  std::filesystem::create_directory("world");
  Get().m_Thread = std::thread(&World::WorldThread);

  ChunkGenerator::Initialize();
  BlockLibrary::Initialize();
}

void World::Finalize() {
  std::cout << "Saving currently loaded chunks..." << std::endl;

  for (auto &chunk : Get().m_ChunkDataMap) {
    chunk.second->~Chunk();
  }

  Get().m_Thread.join();
}

unsigned char World::GetBlock(const glm::ivec3 &worldPos) {
  glm::ivec3 index = GetChunkIndex(worldPos);
  glm::ivec3 chunkCoord = GetChunkCoord(worldPos);

  if (Get().m_ChunkDataMap.find(index) != Get().m_ChunkDataMap.end()) {
    return Get().m_ChunkDataMap.at(index)->GetBlock(chunkCoord);
  } else {
    return 0;
  }
}

void World::SetBlock(const glm::ivec3 &worldPos, unsigned char blockID) {
  if (worldPos.y > CHUNK_HEIGHT) {
    return;
  }

  glm::ivec3 index = GetChunkIndex(worldPos);
  glm::ivec3 chunkCoord = GetChunkCoord(worldPos);

  if (Get().m_ChunkDataMap.find(index) != Get().m_ChunkDataMap.end()) {
    Chunk *chunk = Get().m_ChunkDataMap.at(index);

    chunk->SetBlock(chunkCoord.x, chunkCoord.y, chunkCoord.z, blockID);
    chunk->GenerateMesh();

    Renderer::UpdateMeshInQueue(index);

    return;
  } else {
    return;
  }
}

glm::ivec3 World::GetChunkCoord(const glm::ivec3 &worldPos) {
  glm::ivec3 chunkIndex = GetChunkIndex(worldPos);
  glm::ivec3 chunkPos{worldPos};

  while (chunkPos.x < 0) {
    chunkPos.x += CHUNK_WIDTH;
  }
  while (chunkPos.x > CHUNK_WIDTH - 1) {
    chunkPos.x -= CHUNK_WIDTH;
  }
  while (chunkPos.z < 0) {
    chunkPos.z += CHUNK_WIDTH;
  }
  while (chunkPos.z > CHUNK_WIDTH - 1) {
    chunkPos.z -= CHUNK_WIDTH;
  }

  return chunkPos;
}

glm::ivec3 World::GetChunkIndex(const glm::ivec3 &worldPos) {
  glm::ivec3 chunkIndex(0, 0, 0);

  // if (worldPos.x < 0) {
  //   chunkIndex.x = (worldPos.x - CHUNK_WIDTH) / CHUNK_WIDTH;
  // } else {
    chunkIndex.x = worldPos.x / CHUNK_WIDTH;
  // }

  // if (worldPos.z < 0) {
  //   chunkIndex.z = (worldPos.z - CHUNK_WIDTH) / CHUNK_WIDTH;
  // } else {
    chunkIndex.z = worldPos.z / CHUNK_WIDTH;
  // }

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
       Get().m_ChunkDataMap) {
    if (requestedChunks.find(oldChunk.first) == requestedChunks.end()) {
      Get().m_ChunksToDelete.insert(oldChunk.first);
    }
  }

  for (const glm::ivec3 &newChunk : requestedChunks) {
    if (Get().m_ChunkDataMap.find(newChunk) == Get().m_ChunkDataMap.end()) {
      Get().m_ChunksToCreate.insert(newChunk);
    }
  }

  // Perform full loop through chunks if there is a desync with loaded chunks
  if (Get().m_ChunkDataMap.size() != requestedChunks.size()) {
    for (auto &chunk : Get().m_ChunkDataMap) {
      if (requestedChunks.find(chunk.first) == requestedChunks.end()) {
        Get().m_ChunksToDelete.insert(chunk.first);
      }
    }
  }
}

void World::WorldThread() {
  while (!Engine::ShouldClose()) {
    Get().m_ChunkDataMap.reserve(Get().m_ChunksToCreate.size());

    // Create new chunks
    for (const glm::ivec3 &index : Get().m_ChunksToCreate) {
      // add chunk to data
      Chunk *chunk = new Chunk(index); // heap allocation
      Get().m_ChunkDataMap.insert_or_assign(index, chunk);

      // add mesh to renderer
      Mesh mesh =
          Mesh(Get().m_ChunkDataMap.at(index)->GetVertices(),
               Get().m_ChunkDataMap.at(index)->GetIndices(), &Get().m_Shader);
      Renderer::AddMeshToQueue(index, mesh);
    }
    Get().m_ChunksToCreate.clear();

    // Delete old chunks
    for (const glm::ivec3 &index : Get().m_ChunksToDelete) {
      // remove chunk from data
      delete Get().m_ChunkDataMap.at(index); // heap deletion
      Get().m_ChunkDataMap.erase(index);

      // remove mesh from renderer
      Renderer::DeleteMeshFromQueue(index);
    }
    Get().m_ChunksToDelete.clear();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}