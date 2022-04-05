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

    void GenerateSurface();
    void GenerateTrees();
    void GenerateBedrock();
    void GenerateCaves();

    void GenerateMesh();

    std::vector<Vertex> *GetVertices() { return &m_Vertices; }
    std::vector<unsigned int> *GetIndices() { return &m_Indices; }

    unsigned char GetBlock(glm::ivec3 chunkPos) { return GetBlock(chunkPos.x, chunkPos.y, chunkPos.z); }
    unsigned char GetBlock(int x, int y, int z)
    {
        if (x < 0 || y < 0 || z < 0)
            return 0;
        if (x == CHUNK_WIDTH || y == CHUNK_HEIGHT || z == CHUNK_WIDTH)
            return 0;

        return m_BlockData[(x * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + (z)];
    }

    void SetBlock(int x, int y, int z, unsigned char input)
    {
        if (x < 0 || y < 0 || z < 0)
            return;
        if (x == CHUNK_WIDTH || y == CHUNK_HEIGHT || z == CHUNK_WIDTH)
            return;

        m_BlockData[(x * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + (z)] = input;
    }

    void SetHeight(int x, int z, int y)
    {
        m_HeightData[CHUNK_WIDTH * x + z] = y;
    }

    float GetHeight(int x, int z)
    {
        return m_HeightData[CHUNK_WIDTH * x + z];
    }

private:
    std::vector<unsigned char> m_BlockData; // testing new data format
    std::vector<int> m_HeightData;          // only used during generation, not needed when saving chunk

    glm::ivec3 m_Chunk;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
};