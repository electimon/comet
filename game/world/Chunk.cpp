#include "Chunk.h"

#include "glm/gtc/random.hpp"

#include "Renderer.h"
#include "containers/Vertex.h"
#include "FastNoiseLite.h"

#include "World.h"
#include "Timer.h"
#include "BlockProperties.h"
#include "ChunkGenerator.h"

Chunk::Chunk(glm::ivec3 id)
    : m_Chunk(id),
      m_ChunkSize(World::GetChunkSize()),
      m_ChunkHeight(World::GetChunkHeight()),
      m_Offset(0)
{
    // New data structure
    // m_Blocks.reserve(m_ChunkSize * m_ChunkSize * m_ChunkHeight);
    m_BlockData.resize(m_ChunkSize * m_ChunkSize * m_ChunkHeight); // "3D" array

    // m_SurfaceHeights.reserve(m_ChunkSize * m_ChunkSize);
    m_SurfaceHeightData.resize(m_ChunkSize * m_ChunkSize); // "2D" array

    m_Vertices.reserve(10000);
    m_Indices.reserve(10000);

    GenerateSurface();

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
            height = ChunkGenerator::GetSurfaceNoise((m_Chunk.x * m_ChunkSize) + x, (m_Chunk.z * m_ChunkSize) + z);
            biome = ChunkGenerator::GetBiomeNoise((m_Chunk.x * m_ChunkSize) + x, (m_Chunk.z * m_ChunkSize) + z);

            // height += 1.0f; // 0 to 2
            biome += 1.0f;  // 0 to 2
            height *= 2.5f; // 0 10
            biome *= 50.0f; // 0 10
            int y = static_cast<int>(height + biome);

            SetBlock({x, y, z}, 2); // updates block data
        }
    }
}

void Chunk::GenerateMesh()
{
    unsigned char blockID;
    int worldx, worldy, worldz;

    std::vector<std::vector<int>> textureIndices;
    std::vector<int> Stone{241, 241, 241, 241, 241, 241};
    textureIndices.push_back(Stone);
    std::vector<int> Grass{243, 243, 240, 242, 243, 243};
    textureIndices.push_back(Grass);
    std::vector<int> Dirt{242, 242, 242, 242, 242, 242};
    textureIndices.push_back(Dirt);
    std::vector<int> Water{63, 63, 63, 63, 63, 63};
    textureIndices.push_back(Water);
    std::vector<int> Log{228, 228, 229, 229, 228, 228};
    textureIndices.push_back(Log);
    std::vector<int> Leaves{197, 197, 197, 197, 197, 197};
    textureIndices.push_back(Leaves);
    std::vector<int> Sand{226, 226, 226, 226, 226, 226};
    textureIndices.push_back(Sand);

    for (int x = 0; x < m_ChunkSize; x++)
    {
        for (int y = 0; y < m_ChunkHeight; y++)
        {
            for (int z = 0; z < m_ChunkSize; z++)
            {
                if (GetBlock({x, y, z}) == 0)
                    continue;

                blockID = GetBlock({x, y, z});

                worldx = x + m_ChunkSize * m_Chunk.x;
                worldy = y;
                worldz = z + m_ChunkSize * m_Chunk.z;

                // +X Quad
                if (GetBlock({x + 1, y, z}) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(textureIndices[blockID - 1][0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(textureIndices[blockID - 1][0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(textureIndices[blockID - 1][0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(textureIndices[blockID - 1][0]), {+1.0f, 0.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // -X Quad
                if (GetBlock({x - 1, y, z}) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(textureIndices[blockID - 1][1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(textureIndices[blockID - 1][1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(textureIndices[blockID - 1][1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(textureIndices[blockID - 1][1]), {-1.0f, 0.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // +Y Quad
                if (GetBlock({x, y + 1, z}) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopLeft(textureIndices[blockID - 1][2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopRight(textureIndices[blockID - 1][2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetBottomRight(textureIndices[blockID - 1][2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetBottomLeft(textureIndices[blockID - 1][2]), {0.0f, +1.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // -Y Quad
                if (GetBlock({x, y - 1, z}) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(textureIndices[blockID - 1][3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(textureIndices[blockID - 1][3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(textureIndices[blockID - 1][3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(textureIndices[blockID - 1][3]), {0.0f, -1.0f, 0.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // +Z Quad
                if (GetBlock({x, y, z + 1}) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(textureIndices[blockID - 1][4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopLeft(textureIndices[blockID - 1][4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomLeft(textureIndices[blockID - 1][4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(textureIndices[blockID - 1][4]), {0.0f, 0.0f, +1.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }

                // -Z Quad
                if (GetBlock({x, y, z - 1}) == 0)
                {
                    m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopRight(textureIndices[blockID - 1][5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomRight(textureIndices[blockID - 1][5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(textureIndices[blockID - 1][5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(textureIndices[blockID - 1][5]), {0.0f, 0.0f, -1.0f}, 1.0f),
                        });
                    m_Offset += 4;
                }
            }
        }
    }
}
