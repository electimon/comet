#include "Chunk.h"

Chunk::Chunk(glm::ivec3 id)
    : m_Chunk(id),
      m_ChunkSize(16),
      m_ChunkHeight(1)
{
    std::cout << "Chunk::Chunk()" << std::endl;
    m_Blocks.reserve(m_ChunkSize * m_ChunkSize * m_ChunkHeight);
    FillChunk();
    GenerateMesh();
    // p_World->p_Engine->GetRenderer()->AddToMeshQueue(&m_Mesh);
}

Chunk::~Chunk()
{
    std::cout << "Chunk::~Chunk()" << std::endl;
}

void Chunk::SetBlock(glm::ivec3 coordinate, Block &block)
{
    m_Blocks.insert_or_assign(coordinate, block);
}

void Chunk::FillChunk()
{
    for (int i = 0; i < m_ChunkSize; i++)
    {
        for (int j = 0; j < m_ChunkHeight; j++)
        {
            for (int k = 0; k < m_ChunkSize; k++)
            {
                int blockID = 1;
                // int blockID = glm::linearRand(0, 1);
                if (blockID == 0)
                {
                    continue;
                }
                SetBlock(glm::ivec3(i, j, k), Block(blockID));
            }
        }
    }
}

void Chunk::GenerateMesh()
{
    std::vector<Vertex> m_Vertices;
    m_Vertices.reserve(8 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);
    std::vector<unsigned int> m_Indices;
    m_Indices.reserve(36 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);

    unsigned int offset = 0;

    float red = glm::linearRand(0.0f, 1.0f);
    float green = glm::linearRand(0.0f, 1.0f);
    float blue = glm::linearRand(0.0f, 1.0f);

    int xOffset = m_Chunk.x * m_ChunkSize;
    int zOffset = m_Chunk.z * m_ChunkSize;

    for (int xRelPos = 0; xRelPos < m_ChunkSize; xRelPos++)
    {
        for (int yRelPos = 0; yRelPos < m_ChunkHeight; yRelPos++)
        {
            for (int zRelPos = 0; zRelPos < m_ChunkSize; zRelPos++)
            {
                int xPos = xRelPos + xOffset;
                int yPos = yRelPos;
                int zPos = zRelPos + zOffset;

                if (m_Blocks.find(glm::ivec3(xRelPos, yRelPos, zRelPos)) == m_Blocks.end())
                {
                    continue;
                }

                if (m_Blocks.find(glm::ivec3(xRelPos, yRelPos, zRelPos + 1)) == m_Blocks.end())
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset}); // front
                }
                if (m_Blocks.find(glm::ivec3(xRelPos + 1, yRelPos, zRelPos)) == m_Blocks.end())
                {
                    m_Indices.insert(m_Indices.end(), {1 + offset, 5 + offset, 6 + offset, 6 + offset, 2 + offset, 1 + offset}); // right
                }
                if (m_Blocks.find(glm::ivec3(xRelPos, yRelPos, zRelPos - 1)) == m_Blocks.end())
                {
                    m_Indices.insert(m_Indices.end(), {5 + offset, 4 + offset, 7 + offset, 7 + offset, 6 + offset, 5 + offset}); // back
                }
                if (m_Blocks.find(glm::ivec3(xRelPos - 1, yRelPos, zRelPos)) == m_Blocks.end())
                {
                    m_Indices.insert(m_Indices.end(), {4 + offset, 0 + offset, 3 + offset, 3 + offset, 7 + offset, 4 + offset}); // left
                }
                if (m_Blocks.find(glm::ivec3(xRelPos, yRelPos - 1, zRelPos)) == m_Blocks.end())
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 4 + offset, 5 + offset, 5 + offset, 1 + offset, 0 + offset}); // bottom
                }
                if (m_Blocks.find(glm::ivec3(xRelPos, yRelPos + 1, zRelPos)) == m_Blocks.end())
                {
                    m_Indices.insert(m_Indices.end(), {3 + offset, 2 + offset, 6 + offset, 6 + offset, 7 + offset, 3 + offset}); // top
                }

                m_Vertices.insert(m_Vertices.end(), {Vertex(xPos - 0.5f, yPos - 0.5f, zPos + 0.5f, red, green, blue),
                                                     Vertex(xPos + 0.5f, yPos - 0.5f, zPos + 0.5f, red, green, blue),
                                                     Vertex(xPos + 0.5f, yPos + 0.5f, zPos + 0.5f, red, green, blue),
                                                     Vertex(xPos - 0.5f, yPos + 0.5f, zPos + 0.5f, red, green, blue),
                                                     Vertex(xPos - 0.5f, yPos - 0.5f, zPos - 0.5f, red, green, blue),
                                                     Vertex(xPos + 0.5f, yPos - 0.5f, zPos - 0.5f, red, green, blue),
                                                     Vertex(xPos + 0.5f, yPos + 0.5f, zPos - 0.5f, red, green, blue),
                                                     Vertex(xPos - 0.5f, yPos + 0.5f, zPos - 0.5f, red, green, blue)});

                offset += 8;
            }
        }
    }
    m_Mesh.AddGemoetry(m_Vertices, m_Indices);
}

void Chunk::Redraw()
{
}