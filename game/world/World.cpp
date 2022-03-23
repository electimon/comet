#include "World.h"

#include <thread>

World::World()
{
    std::cout << "World::World()" << std::endl;
    m_Thread = std::thread(&World::WorldThread, this);
}

World::~World()
{
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
}

void World::WorldThread()
{
    m_EndThread = false;

    while (!m_EndThread)
    {
        // Create new chunks
        for (const glm::ivec3 &chunk : m_ChunksToCreate)
        {
            GenerateChunk(chunk);
            Mesh *mesh = new Mesh(m_ChunkDataMap.at(chunk)->GetVertices(), m_ChunkDataMap.at(chunk)->GetIndices(), m_Shader);
            Renderer::AddMeshToQueue(chunk, mesh);
        }
        m_ChunksToCreate.clear();


        // Delete old chunks
        for (const glm::ivec3 &chunk : m_ChunksToDelete)
        {
            delete m_ChunkDataMap.at(chunk);
            m_ChunkDataMap.erase(chunk);
            Renderer::AddMeshToDelete(chunk);
        }
        m_ChunksToDelete.clear();


        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}