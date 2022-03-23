#include "World.h"

#include <thread>

World::World()
{
    std::cout << "World::World()" << std::endl;
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

void World::AddChunkToRenderQueue(const glm::ivec3 &index)
{
    Mesh *mesh = new Mesh(m_ChunkDataMap.at(index)->GetVertices(), m_ChunkDataMap.at(index)->GetIndices(), m_Shader);
    Renderer::AddMesh(index, mesh);
}

void World::ProcessRequestedChunks(const std::unordered_set<glm::ivec3> &requestedChunks)
{
    // std::unordered_set<glm::ivec3> chunksToCreate;
    std::unordered_set<glm::ivec3> chunksToDelete;

    // Loops through the currently loaded chunks and checks if they are
    // present in the requested chunks. If not, added to the chunksToDelete set.
    // Removes from the rendering mesh queue as well. Removing from the map
    // while looping over it is bad.
    for (const std::pair<const glm::ivec3, Chunk *> &oldChunk : m_ChunkDataMap)
    {
        if (requestedChunks.find(oldChunk.first) == requestedChunks.end())
        {
            // old chunk not found in requested chunks, remove remove queue and map
            Renderer::RemoveMesh(oldChunk.first); // handles 'delete' on the mesh on heap
            chunksToDelete.insert(oldChunk.first);
        }
    }

    // Loops through the set of chunksToDelete set and removes them from the
    // currently loaded chunks in the world
    for (const glm::ivec3 &chunk : chunksToDelete)
    {
        // Necessary to delete chunk from heap and map, otherwise memory leak occurs
        delete m_ChunkDataMap.at(chunk); // 'delete on the chunk on heap

        m_ChunkDataMap.erase(chunk);
    }

    std::unordered_set<glm::ivec3> chunksToCreate;

    std::vector<std::thread> threads;
    threads.reserve(25);

    for (const glm::ivec3 &newChunk : requestedChunks)
    {
        if (m_ChunkDataMap.find(newChunk) == m_ChunkDataMap.end())
        {
            threads.push_back(std::thread(&World::GenerateChunk, this, newChunk));
            // new chunk not found in old chunks, add to render queue
            // GenerateChunk(newChunk);
            // threads.push_back(std::thread(&World::GenerateChunk, this, newChunk));
            // AddChunkToRenderQueue(newChunk);
            chunksToCreate.insert(newChunk);
        }
    }

    for (int i = 0; i < threads.size(); i++)
    {
        // if (threads[i].joinable())
            threads[i].join();
    }

    for (const glm::ivec3 &chunkToCreate : chunksToCreate)
    {
        AddChunkToRenderQueue(chunkToCreate);
    }
}