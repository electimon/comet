#pragma once

#include <unordered_map>

#include "glm/gtx/hash.hpp"

#include "Chunk.h"

/*
index = used to describe chunk locations
coordinate = used to describe block locations
*/

class World
{
public:
    World()
    {
        std::cout << "World::World()" << std::endl;
    }
    ~World()
    {
        std::cout << "World::~World()" << std::endl;
    }

    void GenerateChunk(glm::ivec3 index)
    {
        m_Chunks.insert_or_assign(index, Chunk(index));
    }

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk> m_Chunks;
};