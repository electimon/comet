#pragma once

#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include "containers/Mesh.h"

#include "Block.h"

struct Chunk
{
    Chunk(glm::ivec3 id);
    ~Chunk();

    void GenerateSurface();

    void GenerateMesh();

    std::vector<Vertex> *GetVertices() { return &m_Vertices; }
    std::vector<unsigned int> *GetIndices() { return &m_Indices; }

    unsigned char GetBlock(const glm::ivec3 &coord)
    {
        if (coord.x < 0 || coord.y < 0 || coord.z < 0)
            return 0;
        if (coord.x == m_ChunkSize || coord.y == m_ChunkHeight || coord.z == m_ChunkSize)
            return 0;

        int index = (coord.x * m_ChunkHeight * m_ChunkSize) + (coord.y * m_ChunkSize) + (coord.z);

        return m_BlockData[index];
    }

    void SetBlock(const glm::ivec3 &coord, unsigned char input)
    {
        m_BlockData[(coord.x * m_ChunkHeight * m_ChunkSize) + (coord.y * m_ChunkSize) + (coord.z)] = input;
    }

    float GetHeight(const glm::ivec2 &coord)
    {
        return m_BlockData[m_ChunkSize * coord.x + coord.y]; // y is actually z
    }

private:
    std::vector<unsigned char> m_BlockData; // testing new data format
    std::vector<float> m_SurfaceHeightData; // only used during generation, not needed when saving chunk

    glm::ivec3 m_Chunk;

    int m_ChunkSize;
    int m_ChunkHeight;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    unsigned int m_Offset;
};