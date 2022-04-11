#pragma once

#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include "render/Mesh.h"

#include "Block.h"
#include "BlockLibrary.h"
#include "WorldConfig.h"

struct Chunk
{
    Chunk(glm::ivec3 id);
    ~Chunk();

    // For use by world manager
    void Generate();
    void GenerateGeometry();

    void GenerateSurface();
    void GenerateTrees();
    void GenerateBedrock();
    void GenerateCaves();
    void GenerateWater();
    void GenerateSand();

    void GenerateMesh();

    std::vector<Vertex> *GetVertices() { return &m_Vertices; }
    std::vector<unsigned int> *GetIndices() { return &m_Indices; }

    unsigned char GetBlock(glm::ivec3 chunkPos)
    {
        return GetBlock(chunkPos.x, chunkPos.y, chunkPos.z);
    }
    unsigned char GetBlock(int x, int y, int z)
    {
        if (x < 0 || y < 0 || z < 0)
            return 0;
        if (x == CHUNK_WIDTH || y == CHUNK_HEIGHT || z == CHUNK_WIDTH)
            return 0;

        return m_BlockData.at(x * CHUNK_HEIGHT * CHUNK_WIDTH + y * CHUNK_WIDTH + z);
    }
    void SetBlock(const glm::ivec3 &chunkPos, unsigned char blockID)
    {
        SetBlock(chunkPos.x, chunkPos.y, chunkPos.z, blockID);
    }
    void SetBlock(int x, int y, int z, unsigned char blockID)
    {
        m_BlockData.at(x * CHUNK_HEIGHT * CHUNK_WIDTH + y * CHUNK_WIDTH + z) =
            blockID;

        if (m_Generated)
        {
            m_Modified = true;
            m_Generated = false;
        }
    }

    void SetHeight(int x, int z, int y) { m_HeightData[CHUNK_WIDTH * x + z] = y; }

    float GetHeight(int x, int z) { return m_HeightData[CHUNK_WIDTH * x + z]; }

    void MakeModified() { m_Modified = true; }

private:
    // testing new data format
    std::vector<unsigned char> m_BlockData;
    // only used during generation, not needed when saving chunk
    std::vector<int> m_HeightData;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    // Flag to check if the chunk needs to be saved to disk or not
    bool m_Modified = false;
    bool m_Generated = false;

    glm::ivec3 m_Chunk = {0, 0, 0};
};