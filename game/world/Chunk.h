#pragma once

#include <unordered_map>

#include "glm/glm.hpp"

#include "Block.h"

struct Chunk
{
    Chunk(glm::ivec3 id)
        : m_Chunk(id),
          m_ChunkSize(16),
          m_ChunkHeight(64)
    {
        std::cout << "Chunk::Chunk()" << std::endl;
        m_Blocks.reserve(m_ChunkSize * m_ChunkSize * m_ChunkHeight);
        FillChunk(Block(1));
    }
    ~Chunk()
    {
        std::cout << "Chunk::~Chunk()" << std::endl;
    }

    void SetBlock(glm::ivec3 coordinate, Block block)
    {
        m_Blocks.insert_or_assign(coordinate, block);
    }

    void FillChunk(Block block)
    {
        for (int i = 0; i < m_ChunkSize; i++)
        {
            for (int j = 0; j < m_ChunkSize; j++)
            {
                for (int k = 0; k < m_ChunkSize; k++)
                {
                    SetBlock(glm::ivec3(i, j, k), block);
                }
            }
        }
    }

private:
    std::unordered_map<glm::ivec3, Block> m_Blocks;
    glm::ivec3 m_Chunk;
    unsigned int m_ChunkSize;
    unsigned int m_ChunkHeight;
};