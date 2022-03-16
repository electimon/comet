#pragma once

#include <unordered_map>

#include "Chunk.h"

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

    void GenerateChunk(glm::ivec3 coordinate)
    {
        m_Chunks.insert_or_assign(coordinate, Chunk(coordinate));
    }

private:
    // This will be a temporary cache of the loaded chunks.
    // Functionallity to check for saved data on disk will eventually be implemented.
    std::unordered_map<glm::ivec3, Chunk> m_Chunks;
};