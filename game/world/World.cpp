#include "World.h"

World::World()
{
    std::cout << "World::World()" << std::endl;
}
World::~World()
{
    std::cout << "World::~World()" << std::endl;
}

void World::GenerateChunk(glm::ivec3 index)
{
    Chunk *chunk = new Chunk(index);
    m_Chunks.insert_or_assign(index, chunk);
}
void World::GenerateChunk(int x, int y, int z)
{
    GenerateChunk(glm::ivec3(x, y, z));
}

void World::AddChunkToRenderQueue(glm::ivec3 index)
{
    Renderer::AddMesh(index, m_Chunks.at(index)->GetMesh());
}
void World::AddChunkToRenderQueue(int x, int y, int z)
{
    AddChunkToRenderQueue(glm::ivec3(x, y, z));
}

void World::AddShaderToChunk(glm::ivec3 index, unsigned int shader)
{
    m_Chunks.at(index)->GetMesh()->SetShaderID(shader);
}
void World::AddShaderToChunk(int x, int y, int z, unsigned int shader)
{
    AddShaderToChunk(glm::ivec3(x, y, z), shader);
}
