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

    const Block &GetBlock(const glm::ivec3 &coord)
    {
        return m_BlockData[coord.x * m_ChunkHeight * m_ChunkSize + coord.y * m_ChunkSize + coord.z];
    };

    void SetBlock(const glm::ivec3 &coord, const Block &block)
    {
        m_BlockData[coord.x * m_ChunkHeight * m_ChunkSize + coord.y * m_ChunkSize + coord.z] = block;
    }

    int GetHeight(const glm::ivec2 &coord)
    {
        return m_SurfaceData[coord.x * m_ChunkSize + coord.y];
    }

private:
    std::unordered_map<glm::ivec3, Block> m_Blocks;

    std::vector<Block> m_BlockData; // testing new data format
    std::vector<float> m_SurfaceData;

    std::unordered_map<glm::ivec2, int> m_SurfaceHeights;

    glm::ivec3 m_Chunk;

    int m_ChunkSize;
    int m_ChunkHeight;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    unsigned int m_Offset;
};