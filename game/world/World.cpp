#include "World.h"

#include "BlockLibrary.h"
#include "ChunkGenerator.h"
#include "Engine.h"
#include "world/WorldConfig.h"

#include <filesystem>
#include <thread>
#include <cmath>

void World::Initialize()
{
    std::filesystem::create_directory("world");
    Get().m_Thread = std::thread(&World::WorldThread);

    ChunkGenerator::Initialize();
    BlockLibrary::Initialize();
}

void World::Finalize()
{
    std::cout << "Saving currently loaded chunks..." << std::endl;

    for (auto &chunk : Get().m_ChunkDataMap)
    {
        chunk.second->~Chunk();
    }

    Get().m_Thread.join();
}

unsigned char World::GetBlock(const glm::ivec3 &worldPos)
{
    glm::ivec3 index = GetChunkIndex(worldPos);
    glm::ivec3 chunkCoord = GetChunkCoord(worldPos);

    if (Get().m_ChunkDataMap.find(index) != Get().m_ChunkDataMap.end())
    {
        return Get().m_ChunkDataMap.at(index)->GetBlock(chunkCoord);
    }
    else
    {
        return 0;
    }
}

void World::SetBlock(const glm::ivec3 &worldPos, unsigned char blockID)
{
    if (worldPos.y > CHUNK_HEIGHT)
    {
        return;
    }

    glm::ivec3 index = GetChunkIndex(worldPos);
    glm::ivec3 chunkCoord = GetChunkCoord(worldPos);

    if (Get().m_ChunkDataMap.find(index) != Get().m_ChunkDataMap.end())
    {
        Chunk *chunk = Get().m_ChunkDataMap.at(index);

        chunk->SetBlock(chunkCoord.x, chunkCoord.y, chunkCoord.z, blockID);
        chunk->GenerateMesh();

        Renderer::UpdateMeshInQueue(index);

        return;
    }
    else
    {
        return;
    }
}

glm::ivec3 World::GetChunkCoord(const glm::ivec3 &worldPos)
{
    glm::ivec3 chunkIndex = GetChunkIndex(worldPos);
    glm::ivec3 chunkPos = worldPos;

    while (chunkPos.x < 0)
    {
        chunkPos.x += CHUNK_WIDTH;
    }
    while (chunkPos.x > CHUNK_WIDTH - 1)
    {
        chunkPos.x -= CHUNK_WIDTH;
    }
    while (chunkPos.z < 0)
    {
        chunkPos.z += CHUNK_WIDTH;
    }
    while (chunkPos.z > CHUNK_WIDTH - 1)
    {
        chunkPos.z -= CHUNK_WIDTH;
    }

    return chunkPos;
}

glm::ivec3 World::GetChunkIndex(const glm::ivec3 &worldPos)
{
    glm::ivec3 chunkIndex(0, 0, 0);

    chunkIndex.x = std::floor(static_cast<double>(worldPos.x) / static_cast<double>(CHUNK_WIDTH));
    chunkIndex.z = std::floor(static_cast<double>(worldPos.z) / static_cast<double>(CHUNK_WIDTH));

    return chunkIndex;
}

void World::ProcessRequestedChunks(
    const std::unordered_set<glm::ivec3> &requestedChunks)
{
    // Loops through the currently loaded chunks and checks if they are
    // present in the requested chunks. If not, added to the chunksToDelete set.
    // Removes from the rendering mesh queue as well. Removing from the map
    // while looping over it is bad.
    for (const auto &[index, chunk] : Get().m_ChunkDataMap)
    {
        if (requestedChunks.find(index) == requestedChunks.end())
        {
            Get().m_ChunksToDelete.insert(index);
        }
    }

    for (const auto &newChunk : requestedChunks)
    {
        if (Get().m_ChunkDataMap.find(newChunk) == Get().m_ChunkDataMap.end())
        {
            Get().m_ChunksToCreate.insert(newChunk);
        }
    }

    // Perform full loop through chunks if there is a desync with loaded chunks
    if (Get().m_ChunkDataMap.size() != requestedChunks.size())
    {
        for (auto &chunk : Get().m_ChunkDataMap)
        {
            if (requestedChunks.find(chunk.first) == requestedChunks.end())
            {
                Get().m_ChunksToDelete.insert(chunk.first);
            }
        }
    }
}

void World::WorldThread()
{
    while (!Engine::ShouldClose())
    {
        Get().m_ChunkDataMap.reserve(Get().m_ChunksToCreate.size());

        std::unordered_set temp = Get().m_ChunksToCreate;

        for (const auto &index : Get().m_ChunksToCreate)
        {
            // Generate Chunk Data
            Chunk *chunk = new Chunk(index); // heap allocation
            chunk->Generate();
            Get().m_ChunkDataMap.insert_or_assign(index, chunk);
        }

        Get().m_ChunksToCreate.clear();

        for (const auto &index : temp)
        {
            // Generate Chunk Geometry
            Chunk *chunk = Get().m_ChunkDataMap.at(index);
            chunk->GenerateGeometry();
            Mesh mesh = Mesh(chunk->GetVertices(), chunk->GetIndices(), &Get().m_Shader);

            // Adding to Renderer
            Renderer::AddMeshToQueue(index, mesh);
        }

        // Delete old chunks
        for (const auto &index : Get().m_ChunksToDelete)
        {
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