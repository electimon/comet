#pragma once

#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include "containers/Mesh.h"

#include "Block.h"
#include "BlockLibrary.h"

struct Chunk
{
    Chunk(glm::ivec3 id);
    ~Chunk();

    void GenerateSurface();
    void GenerateTrees();

    void GenerateMesh();

    std::vector<Vertex> *GetVertices() { return &m_Vertices; }
    std::vector<unsigned int> *GetIndices() { return &m_Indices; }

    unsigned char GetBlock(int x, int y, int z)
    {
        if (x < 0 || y < 0 || z < 0)
            return 0;
        if (x == m_ChunkSize || y == m_ChunkHeight || z == m_ChunkSize)
            return 0;

        return m_BlockData[(x * m_ChunkHeight * m_ChunkSize) + (y * m_ChunkSize) + (z)];
    }

    void SetBlock(int x, int y, int z, unsigned char input)
    {
        if (x < 0 || y < 0 || z < 0)
            return;
        if (x == m_ChunkSize || y == m_ChunkHeight || z == m_ChunkSize)
            return;

        m_BlockData[(x * m_ChunkHeight * m_ChunkSize) + (y * m_ChunkSize) + (z)] = input;
    }

    void SetHeight(int x, int z, int y)
    {
        m_HeightData[m_ChunkSize * x + z] = y;
    }

    float GetHeight(int x, int z)
    {
        return m_HeightData[m_ChunkSize * x + z];
    }

private:
    std::vector<unsigned char> m_BlockData; // testing new data format
    std::vector<int> m_HeightData; // only used during generation, not needed when saving chunk

    glm::ivec3 m_Chunk;

    int m_ChunkSize;
    int m_ChunkHeight;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    unsigned int m_Offset;
};