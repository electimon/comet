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

    const float &GetBlock(const glm::ivec3 &coord)
    {
        return m_BlockData[coord.x * m_ChunkHeight * m_ChunkSize + coord.y * m_ChunkSize + coord.z];
    };

    void SetBlock(const glm::ivec3 &coord, float input)
    {
        m_BlockData[coord.x * m_ChunkHeight * m_ChunkSize + coord.y * m_ChunkSize + coord.z] = input;
    }

    float GetHeight(const glm::ivec2 &coord)
    {
        return m_SurfaceHeightData[m_ChunkSize * coord.x + coord.y]; // y is actually z
    }

    void DeleteGeometry()
    {
        m_Vertices.clear();
        m_Vertices.shrink_to_fit();

        m_Indices.clear();
        m_Indices.shrink_to_fit();
    }

private:
    std::vector<float> m_BlockData;         // testing new data format
    std::vector<float> m_SurfaceHeightData; // only used during generation, not needed when saving chunk

    //
    std::unordered_map<glm::ivec3, Block> m_Blocks;
    std::unordered_map<glm::ivec2, float> m_SurfaceHeights;

    glm::ivec3 m_Chunk;

    int m_ChunkSize;
    int m_ChunkHeight;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    unsigned int m_Offset;
};