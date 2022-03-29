#include "World.h"

#include "Engine.h"

#include "ChunkGenerator.h"
#include "BlockLibrary.h"

#include <thread>

World::World()
{
    std::cout << "World::World()" << std::endl;
    m_Thread = std::thread(&World::WorldThread, this);

    ChunkGenerator::Initialize();
    BlockLibrary::Initialize();
}

World::~World()
{
    m_Thread.join();
    std::cout << "World::~World()" << std::endl;
}

void World::GenerateChunk(const glm::ivec3 &index)
{
    Chunk *chunk = new Chunk(index);
    m_ChunkDataMap.insert_or_assign(index, chunk);
}

void World::ProcessRequestedChunks(const std::unordered_set<glm::ivec3> &requestedChunks)
{
    // Loops through the currently loaded chunks and checks if they are
    // present in the requested chunks. If not, added to the chunksToDelete set.
    // Removes from the rendering mesh queue as well. Removing from the map
    // while looping over it is bad.
    for (const std::pair<const glm::ivec3, Chunk *> &oldChunk : m_ChunkDataMap)
    {
        if (requestedChunks.find(oldChunk.first) == requestedChunks.end())
        {
            m_ChunksToDelete.insert(oldChunk.first);
        }
    }

    for (const glm::ivec3 &newChunk : requestedChunks)
    {
        if (m_ChunkDataMap.find(newChunk) == m_ChunkDataMap.end())
        {
            m_ChunksToCreate.insert(newChunk);
        }
    }

    // Perform full loop through chunks if there is a desync with loaded chunks
    if (m_ChunkDataMap.size() != requestedChunks.size())
    {
        for (auto &chunk : m_ChunkDataMap)
        {
            if (requestedChunks.find(chunk.first) == requestedChunks.end())
            {
                m_ChunksToDelete.insert(chunk.first);
            }
        }
    }
}

void World::WorldThread()
{
    while (!Engine::ShouldClose())
    {

        // SetBlock(0, 100 + i, 0, 1);
        // i++;

        // Create new chunks
        for (const glm::ivec3 &index : m_ChunksToCreate)
        {
            // add chunk to data
            Chunk *chunk = new Chunk(index); // heap allocation
            m_ChunkDataMap.insert_or_assign(index, chunk);

            // add mesh to renderer
            Mesh mesh = Mesh(m_ChunkDataMap.at(index)->GetVertices(), m_ChunkDataMap.at(index)->GetIndices(), m_Shader);
            Renderer::AddMeshToQueue(index, mesh);
        }
        m_ChunksToCreate.clear();

        // Delete old chunks
        for (const glm::ivec3 &index : m_ChunksToDelete)
        {
            // remove chunk from data
            delete m_ChunkDataMap.at(index); // heap deletion
            m_ChunkDataMap.erase(index);

            // remove mesh from renderer
            Renderer::DeleteMeshFromQueue(index);
        }
        m_ChunksToDelete.clear();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}