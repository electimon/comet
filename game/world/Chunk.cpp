#include "Chunk.h"

#include "glm/gtc/random.hpp"

#include "Renderer.h"
#include "containers/Vertex.h"
#include "FastNoiseLite.h"
#include "TextureMap.h"

#include "World.h"
#include "Timer.h"
#include "ChunkGenerator.h"

Chunk::Chunk(glm::ivec3 id)
    : m_Chunk(id),
      m_ChunkSize(World::GetChunkSize()),
      m_ChunkHeight(World::GetChunkHeight()),
      m_Offset(0)
{
    // New data structure
    m_BlockData.resize(m_ChunkSize * m_ChunkSize * m_ChunkHeight); // "3D" array of block id values
    m_HeightData.resize(m_ChunkSize * m_ChunkSize);                // "2D" array of height values

    m_Vertices.reserve(10000);
    m_Indices.reserve(10000);

    GenerateSurface();
    // GenerateTrees();
    // GenerateCaves();

    GenerateMesh();
}

Chunk::~Chunk()
{
}

void Chunk::GenerateSurface()
{
    float height = 0.0f;
    float biome = 0.0f;

    // Increasing the range of surface generation to go one beyond the chunk size
    // makes it so that when generating the geometry for the chunk, it is able to
    // check for surrounding blocks on the edges of chunks correctly and doesn't push
    // indices between chunks to the GPU.

    // 0 -> 15
    // -1 -> 16
    for (int x = 0; x < m_ChunkSize; x++)
    {
        for (int z = 0; z < m_ChunkSize; z++)
        {
            // Calculating a surface height with the noise
            height = ChunkGenerator::GetMediumNoise((m_Chunk.x * m_ChunkSize) + x, (m_Chunk.z * m_ChunkSize) + z);
            biome = ChunkGenerator::GetSlowNoise((m_Chunk.x * m_ChunkSize) + x, (m_Chunk.z * m_ChunkSize) + z);

            // height += 1.0f; // 0 to 2
            biome += 1.0f;  // 0 to 2
            height *= 2.5f; // 0 10
            biome *= 50.0f; // 0 10
            int y = static_cast<int>(height + biome);

            SetHeight(x, z, y);

            SetBlock(x, y, z, 2);     // grass
            SetBlock(x, y - 1, z, 3); // dirt
            SetBlock(x, y - 2, z, 3); // dirt
            SetBlock(x, y - 3, z, 3); // dirt

            // fill chunk under dirt with stone
            for (int i = 0; i < y - 3; i++)
            {
                SetBlock(x, i, z, 1);
            }
        }
    }
}

void Chunk::GenerateTrees()
{
    float noise1;
    float noise2;
    int y;

    for (int x = 2; x < m_ChunkSize - 2; x++)
    {
        for (int z = 2; z < m_ChunkSize - 2; z++)
        {
            y = GetHeight(x, z);

            if (GetBlock(x, y - 1, z) == 0)
                continue;

            noise1 = ChunkGenerator::GetFastNoise((m_Chunk.x * m_ChunkSize) + x, (m_Chunk.y * m_ChunkHeight) + y, (m_Chunk.z * m_ChunkSize) + z);
            noise2 = ChunkGenerator::GetMediumNoise((m_Chunk.x * m_ChunkSize) + x, (m_Chunk.z * m_ChunkSize) + z);

            // std::cout << "WhiteNoise: " << noise1 << " CellNoise: " << noise2 << std::endl;

            if (noise1 > 0.8f && noise2 > 0.5f)
            {
                SetBlock(x, y + 1, z, 5);
                SetBlock(x, y + 2, z, 5);
                if (noise1 > 0.9f)
                {
                    SetBlock(x, y + 3, z, 5);
                    y += 1;
                }

                SetBlock(x - 2, y + 3, z - 1, 6);
                SetBlock(x - 2, y + 3, z, 6);
                SetBlock(x - 2, y + 3, z + 1, 6);

                SetBlock(x - 1, y + 3, z - 2, 6);
                SetBlock(x - 1, y + 3, z - 1, 6);
                SetBlock(x - 1, y + 3, z, 6);
                SetBlock(x - 1, y + 3, z + 1, 6);
                SetBlock(x - 1, y + 3, z + 2, 6);

                SetBlock(x, y + 3, z - 2, 6);
                SetBlock(x, y + 3, z - 1, 6);

                SetBlock(x, y + 3, z + 1, 6);
                SetBlock(x, y + 3, z + 2, 6);

                SetBlock(x + 1, y + 3, z - 2, 6);
                SetBlock(x + 1, y + 3, z - 1, 6);
                SetBlock(x + 1, y + 3, z, 6);
                SetBlock(x + 1, y + 3, z + 1, 6);
                SetBlock(x + 1, y + 3, z + 2, 6);

                SetBlock(x + 2, y + 3, z - 1, 6);
                SetBlock(x + 2, y + 3, z, 6);
                SetBlock(x + 2, y + 3, z + 1, 6);
            }
        }
    }
}

void Chunk::GenerateMesh()
{
    unsigned char blockID;
    int worldx, worldy, worldz;

    for (int x = 0; x < m_ChunkSize; x++)
    {
        for (int y = 0; y < m_ChunkHeight; y++)
        {
            for (int z = 0; z < m_ChunkSize; z++)
            {
                if (GetBlock(x, y, z) == 0)
                    continue;

                blockID = GetBlock(x, y, z);
                std::vector<int> blockIndices = BlockLibrary::GetIndices(blockID);

                worldx = x + m_ChunkSize * m_Chunk.x;
                worldy = y;
                worldz = z + m_ChunkSize * m_Chunk.z;

                // +X Quad
                if (GetBlock(x + 1, y, z) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // -X Quad
                if (GetBlock(x - 1, y, z) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // +Y Quad
                if (GetBlock(x, y + 1, z) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetBottomRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetBottomLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // -Y Quad
                if (GetBlock(x, y - 1, z) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // +Z Quad
                if (GetBlock(x, y, z + 1) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // -Z Quad
                if (GetBlock(x, y, z - 1) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }
            }
        }
    }
}
