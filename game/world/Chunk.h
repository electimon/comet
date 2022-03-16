#pragma once

#include <unordered_map>

#include "glm/glm.hpp"

struct Chunk
{
    Chunk(glm::ivec3 id)
        : m_Chunk(id)
    {
    }
    ~Chunk();

private:
    std::unordered_map m_Blocks;
    glm::ivec3 m_Chunk;
};