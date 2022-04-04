#pragma once

#include <unordered_map>
#include <unordered_set>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include "render/containers/Shader.h"

#include "render/Renderer.h"
#include "Chunk.h"

class World
{
public:
    World();
    ~World();

    unsigned char GetBlock(const glm::vec3 &worldPos)
    {
        glm::ivec3 index = GetChunkIndexFromWorldCoord(worldPos);
        glm::ivec3 chunkCoord = GetChunkCoordFromWorldCoord(worldPos);

        if (m_ChunkDataMap.find(index) != m_ChunkDataMap.end())
        {
            return m_ChunkDataMap.at(index)->GetBlock(chunkCoord);
        }
        else
        {
            return 0;
        }
    }

    bool SetBlock(int x, int y, int z, unsigned char blockID) { return SetBlock({x, y, z}, blockID); }

    bool SetBlock(const glm::vec3 &worldPos, unsigned char blockID)
    {
        if (worldPos.y > CHUNK_HEIGHT)
            return false;

        glm::ivec3 index = GetChunkIndexFromWorldCoord(worldPos);
        glm::ivec3 chunkCoord = GetChunkCoordFromWorldCoord(worldPos);

        if (m_ChunkDataMap.find(index) != m_ChunkDataMap.end())
        {
            Chunk *chunk = m_ChunkDataMap.at(index);

            chunk->SetBlock(chunkCoord.x, chunkCoord.y, chunkCoord.z, blockID);
            chunk->GenerateMesh();

            Renderer::UpdateMeshInQueue(index);

            std::cout << "Successfully placed block." << std::endl;
            return true;
        }
        else
        {
            std::cout << "Failed to placed block." << std::endl;
            return false;
        }
    }

    static glm::ivec3 GetChunkCoordFromWorldCoord(const glm::vec3 &worldPos)
    {
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

    static glm::ivec3 GetChunkIndexFromWorldCoord(const glm::vec3 &worldPos)
    {
        glm::ivec3 chunkIndex(0, 0, 0);

        if (worldPos.x + 0.5f < 0.0f)
        {
            chunkIndex.x = (worldPos.x - CHUNK_WIDTH) / CHUNK_WIDTH;
        }
        else
        {
            chunkIndex.x = worldPos.x / CHUNK_WIDTH;
        }

        if (worldPos.z + 0.5f < 0.0f)
        {
            chunkIndex.z = (worldPos.z - CHUNK_WIDTH) / CHUNK_WIDTH;
        }
        else
        {
            chunkIndex.z = worldPos.z / CHUNK_WIDTH;
        }

        return chunkIndex;
    }

    void GenerateChunk(const glm::ivec3 &index);

    void ProcessRequestedChunks(const std::unordered_set<glm::ivec3> &chunks);

    const Shader &GetShader() { return m_Shader; }
    void SetShader(const Shader &shader) { m_Shader = shader; }

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk *> m_ChunkDataMap;

    std::unordered_set<glm::ivec3> m_ChunksToDelete;
    std::unordered_set<glm::ivec3> m_ChunksToCreate;

    Shader m_Shader;

    std::thread m_Thread;
    void WorldThread();
};