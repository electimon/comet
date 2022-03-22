#include "Chunk.h"

#include "glm/gtc/random.hpp"

#include "FastNoise.h"

#include "Renderer.h"
#include "containers/Vertex.h"

#include "World.h"

Chunk::Chunk(glm::ivec3 id)
    : m_Chunk(id),
      m_ChunkSize(World::GetChunkSize()),
      m_ChunkHeight(World::GetChunkHeight())
{
    std::cout << "Chunk::Chunk()" << std::endl;
    m_Blocks.reserve(m_ChunkSize * m_ChunkSize);
    FillChunk();
    GenerateMesh();
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
    FastNoise noise;
    // noise.SetFrequency(0.02);
    noise.SetNoiseType(FastNoise::NoiseType::Simplex);
    FastNoise noise2;
    noise2.SetNoiseType(FastNoise::NoiseType::WhiteNoise);

    for (int x = 0; x < m_ChunkSize; x++)
    {
        for (int z = 0; z < m_ChunkSize; z++)
        {
            float noisevalue = noise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z);
            float height = (noisevalue + 1.0f) / 2.0f;

            height *= float(World::GetChunkHeight() / 2); // scale
            height = int(height);

            for (int y = 0; y < height - 3; y++)
            {
                SetBlock(glm::ivec3(x, y, z), Block(1)); // stone
            }
            {
                SetBlock(glm::ivec3(x, height - 3, z), Block(3)); // dirt
                SetBlock(glm::ivec3(x, height - 2, z), Block(3)); // dirt
                SetBlock(glm::ivec3(x, height - 1, z), Block(3)); // dirt

                SetBlock(glm::ivec3(x, height, z), Block(2)); // grass
            }

            if (height < World::GetWaterHeight())
            {
                // fill in with water surface level is below the world water height
                for (int y = height; y < World::GetWaterHeight(); y++)
                {
                    SetBlock(glm::ivec3(x, y, z), Block(4)); // water
                }
            }
            else
            {
                // Roll to place a tree
                float noisevalue2 = noise2.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z);
                if (noisevalue2 > 0.995f)
                {
                    Chunk::PlaceTree(x, height, z);
                }
            }
        }
    }
}

void Chunk::PlaceTree(int x, int y, int z)
{
    SetBlock(glm::ivec3(x, y, z), Block(0));     // test block
    SetBlock(glm::ivec3(x, y + 1, z), Block(0)); // test block
    SetBlock(glm::ivec3(x, y + 2, z), Block(0)); // test block

    for (int i = x - 2; i < x + 3; i++)
    {
        for (int j = z - 2; j < z + 3; j++)
        {
            if ((i == x - 2 && j == z - 2) || (i == x + 2 && j == z + 2) || (i == x - 2 && j == z + 2) || (i == x + 2 && j == z + 2))
                continue;

            SetBlock(glm::ivec3(i, y + 3, j), Block(0)); // test block
            SetBlock(glm::ivec3(i, y + 4, j), Block(0)); // test block
        }
    }

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = z - 1; j < z + 2; j++)
        {
            SetBlock(glm::ivec3(i, y + 5, j), Block(0)); // test block
        }
    }

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = z - 1; j < z + 2; j++)
        {
            if ((i == x - 1 && j == z - 1) || (i == x + 1 && j == z + 1) || (i == x - 1 && j == z + 1) || (i == x + 1 && j == z - 1))
                continue;

            SetBlock(glm::ivec3(i, y + 6, j), Block(0)); // test block
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

    float r;
    float g;
    float b;
    float a;

    int xOffset = m_Chunk.x * m_ChunkSize;
    int zOffset = m_Chunk.z * m_ChunkSize;

    for (int xRel = 0; xRel < m_ChunkSize; xRel++)
    {
        for (int yRel = 0; yRel < m_ChunkHeight; yRel++)
        {
            for (int zRel = 0; zRel < m_ChunkSize; zRel++)
            {
                int x = xRel + xOffset;
                int y = yRel;
                int z = zRel + zOffset;

                if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel)) == m_Blocks.end())
                {
                    continue;
                }

                // White block
                if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 0)
                {
                    r = 1.0f;
                    g = 1.0f;
                    b = 1.0f;
                    a = 1.0f;
                }

                // Stone
                if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 1)
                {
                    r = 94.0f / 255.0f;
                    g = 94.0f / 255.0f;
                    b = 94.0f / 255.0f;
                    a = 1.0f;
                }

                // Grass
                if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 2)
                {
                    r = 26.0f / 255.0f;
                    g = 166.0f / 255.0f;
                    b = 18.0f / 255.0f;
                    a = 1.0f;
                }

                // Dirt
                if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 3)
                {
                    r = 92.0f / 255.0f;
                    g = 68.0f / 255.0f;
                    b = 46.0f / 255.0f;
                    a = 1.0f;
                }

                // Water
                if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 4)
                {
                    r = 66.0f / 255.0f;
                    g = 173.0f / 255.0f;
                    b = 245.0f / 255.0f;
                    a = 0.5f;
                }

                int blockPosX = 0;
                int blockNegX = 0;
                int blockPosY = 0;
                int blockNegY = 0;
                int blockPosZ = 0;
                int blockNegZ = 0;

                if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() != 4)
                {
                    if (m_Blocks.find(glm::ivec3(xRel + 1, yRel, zRel)) != m_Blocks.end())
                        blockPosX = m_Blocks.at(glm::ivec3(xRel + 1, yRel, zRel)).GetID();
                    if (m_Blocks.find(glm::ivec3(xRel - 1, yRel, zRel)) != m_Blocks.end())
                        blockNegX = m_Blocks.at(glm::ivec3(xRel - 1, yRel, zRel)).GetID();
                    if (m_Blocks.find(glm::ivec3(xRel, yRel + 1, zRel)) != m_Blocks.end())
                        blockPosY = m_Blocks.at(glm::ivec3(xRel, yRel + 1, zRel)).GetID();
                    if (m_Blocks.find(glm::ivec3(xRel, yRel - 1, zRel)) != m_Blocks.end())
                        blockNegY = m_Blocks.at(glm::ivec3(xRel, yRel - 1, zRel)).GetID();
                    if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel + 1)) != m_Blocks.end())
                        blockPosZ = m_Blocks.at(glm::ivec3(xRel, yRel, zRel + 1)).GetID();
                    if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel - 1)) != m_Blocks.end())
                        blockNegZ = m_Blocks.at(glm::ivec3(xRel, yRel, zRel - 1)).GetID();
                }

                if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel + 1)) == m_Blocks.end() || blockPosZ == 4)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset}); // front
                }
                if (m_Blocks.find(glm::ivec3(xRel + 1, yRel, zRel)) == m_Blocks.end() || blockPosX == 4)
                {
                    m_Indices.insert(m_Indices.end(), {1 + offset, 5 + offset, 6 + offset, 6 + offset, 2 + offset, 1 + offset}); // right
                }
                if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel - 1)) == m_Blocks.end() || blockNegZ == 4)
                {
                    m_Indices.insert(m_Indices.end(), {5 + offset, 4 + offset, 7 + offset, 7 + offset, 6 + offset, 5 + offset}); // back
                }
                if (m_Blocks.find(glm::ivec3(xRel - 1, yRel, zRel)) == m_Blocks.end() || blockNegX == 4)
                {
                    m_Indices.insert(m_Indices.end(), {4 + offset, 0 + offset, 3 + offset, 3 + offset, 7 + offset, 4 + offset}); // left
                }
                if (m_Blocks.find(glm::ivec3(xRel, yRel - 1, zRel)) == m_Blocks.end() || blockNegY == 4)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 4 + offset, 5 + offset, 5 + offset, 1 + offset, 0 + offset}); // bottom
                }
                if (m_Blocks.find(glm::ivec3(xRel, yRel + 1, zRel)) == m_Blocks.end() || blockPosY == 4)
                {
                    m_Indices.insert(m_Indices.end(), {3 + offset, 2 + offset, 6 + offset, 6 + offset, 7 + offset, 3 + offset}); // top
                }

                m_Vertices.insert(m_Vertices.end(), {Vertex(x - 0.5f, y - 0.5f, z + 0.5f, 0.7f * r, 0.7f * g, 0.7f * b, a),
                                                     Vertex(x + 0.5f, y - 0.5f, z + 0.5f, 0.7f * r, 0.7f * g, 0.7f * b, a),
                                                     Vertex(x + 0.5f, y + 0.5f, z + 0.5f, r, g, b, a),
                                                     Vertex(x - 0.5f, y + 0.5f, z + 0.5f, r, g, b, a),
                                                     Vertex(x - 0.5f, y - 0.5f, z - 0.5f, 0.7f * r, 0.7f * g, 0.7f * b, a),
                                                     Vertex(x + 0.5f, y - 0.5f, z - 0.5f, 0.7f * r, 0.7f * g, 0.7f * b, a),
                                                     Vertex(x + 0.5f, y + 0.5f, z - 0.5f, r, g, b, a),
                                                     Vertex(x - 0.5f, y + 0.5f, z - 0.5f, r, g, b, a)});

                offset += 8;
            }
        }
    }
    m_Mesh.AddGemoetry(m_Vertices, m_Indices);
}

void Chunk::Redraw()
{
}