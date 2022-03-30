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
    : m_Chunk(id)
{
    // New data structure
    m_BlockData.resize(CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT); // "3D" array of block id values
    m_HeightData.resize(CHUNK_WIDTH * CHUNK_WIDTH);               // "2D" array of height values

    m_Vertices.reserve(100000);
    m_Indices.reserve(100000);

    GenerateSurface();

    GenerateBedrock();
    // GenerateCaves();
    GenerateTrees();
    // GenerateCaves();

    GenerateMesh();

    m_HeightData.clear();
    m_HeightData.shrink_to_fit();
}

Chunk::~Chunk()
{
}

void Chunk::GenerateSurface()
{
    float height = 0.0f;
    int y = 0;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_WIDTH; z++)
        {
            // Calculating a surface height with the noise
            height =
                1.0000f * ChunkGenerator::GetOpenSimplex2_1f((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z) +
                0.5000f * ChunkGenerator::GetOpenSimplex2_2f((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z) +
                0.2500f * ChunkGenerator::GetOpenSimplex2_4f((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z) +
                0.1250f * ChunkGenerator::GetOpenSimplex2_8f((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z) +
                0.0625f * ChunkGenerator::GetOpenSimplex2_16f((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            height *= 20.0f;
            height += 40.0f;
            y = static_cast<int>(height);

            SetHeight(x, z, height);

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

    for (int x = 2; x < CHUNK_WIDTH - 2; x++)
    {
        for (int z = 2; z < CHUNK_WIDTH - 2; z++)
        {
            y = GetHeight(x, z);

            if (GetBlock(x, y - 1, z) == 0)
                continue;

            noise1 = ChunkGenerator::GetFastNoise((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            noise2 = ChunkGenerator::GetMediumNoise((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);

            if (noise1 > 0.9f && noise2 > 0.5f)
            {
                SetBlock(x, y + 1, z, 5);
                SetBlock(x, y + 2, z, 5);
                if (noise1 > 0.95f)
                {
                    SetBlock(x, y + 3, z, 5);
                    y += 1;
                }
                {
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
                    SetBlock(x, y + 3, z, 5);
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
                {
                    SetBlock(x - 2, y + 4, z - 1, 6);
                    SetBlock(x - 2, y + 4, z, 6);
                    SetBlock(x - 2, y + 4, z + 1, 6);

                    SetBlock(x - 1, y + 4, z - 2, 6);
                    SetBlock(x - 1, y + 4, z - 1, 6);
                    SetBlock(x - 1, y + 4, z, 6);
                    SetBlock(x - 1, y + 4, z + 1, 6);
                    SetBlock(x - 1, y + 4, z + 2, 6);

                    SetBlock(x, y + 4, z - 2, 6);
                    SetBlock(x, y + 4, z - 1, 6);
                    SetBlock(x, y + 4, z, 6);
                    SetBlock(x, y + 4, z + 1, 6);
                    SetBlock(x, y + 4, z + 2, 6);

                    SetBlock(x + 1, y + 4, z - 2, 6);
                    SetBlock(x + 1, y + 4, z - 1, 6);
                    SetBlock(x + 1, y + 4, z, 6);
                    SetBlock(x + 1, y + 4, z + 1, 6);
                    SetBlock(x + 1, y + 4, z + 2, 6);

                    SetBlock(x + 2, y + 4, z - 1, 6);
                    SetBlock(x + 2, y + 4, z, 6);
                    SetBlock(x + 2, y + 4, z + 1, 6);
                }
                {
                    SetBlock(x - 1, y + 5, z, 6);

                    SetBlock(x, y + 5, z - 1, 6);
                    SetBlock(x, y + 5, z, 6);
                    SetBlock(x, y + 5, z + 1, 6);

                    SetBlock(x + 1, y + 5, z, 6);
                }
                {
                    SetBlock(x - 1, y + 6, z, 6);

                    SetBlock(x, y + 6, z - 1, 6);
                    SetBlock(x, y + 6, z, 6);
                    SetBlock(x, y + 6, z + 1, 6);

                    SetBlock(x + 1, y + 6, z, 6);
                }
            }
        }
    }
}

void Chunk::GenerateBedrock()
{
    float noise;
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_WIDTH; z++)
        {
            noise = ChunkGenerator::GetFastNoise(x, z);

            SetBlock(x, 0, z, 8);

            if (noise > 0.20f)
                SetBlock(x, 1, z, 8);
            else
                continue;
            if (noise > 0.40f)
                SetBlock(x, 2, z, 8);
            else
                continue;
            if (noise > 0.60f)
                SetBlock(x, 3, z, 8);
            else
                continue;
            if (noise > 0.80f)
                SetBlock(x, 4, z, 8);
        }
    }
}

void Chunk::GenerateCaves()
{
    float noise;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_WIDTH; z++)
            {
                if (GetBlock(x, y, z) != 1)
                    continue;

                noise = ChunkGenerator::GetCaveNoise(x + m_Chunk.x * CHUNK_WIDTH, y, z + m_Chunk.z * CHUNK_WIDTH);

                if (noise > 0.8f)
                    SetBlock(x, y, z, 0);
            }
        }
    }
}

void Chunk::GenerateMesh()
{
    m_Vertices.clear();
    m_Indices.clear();
    unsigned int offset = 0;

    unsigned char blockID;
    int worldx, worldy, worldz;

    bool px, nx, py, ny, pz, nz;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_WIDTH; z++)
            {
                if (GetBlock(x, y, z) == 0)
                    continue;

                blockID = GetBlock(x, y, z);
                std::vector<unsigned char> blockIndices = BlockLibrary::GetIndices(blockID);

                worldx = x + CHUNK_WIDTH * m_Chunk.x;
                worldy = y;
                worldz = z + CHUNK_WIDTH * m_Chunk.z;

                px = GetBlock(x + 1, y, z) == 0;
                nx = GetBlock(x - 1, y, z) == 0;
                py = GetBlock(x, y + 1, z) == 0;
                ny = GetBlock(x, y - 1, z) == 0;
                pz = GetBlock(x, y, z + 1) == 0;
                nz = GetBlock(x, y, z - 1) == 0;

                if (blockID == 6)
                {
                    px = true;
                    nx = true;
                    py = true;
                    ny = true;
                    pz = true;
                    nz = true;
                }

                // +X Quad
                if (px)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                        });
                    offset += 4;
                }

                // -X Quad
                if (nx)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                        });
                    offset += 4;
                }

                // +Y Quad
                if (py)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetBottomRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetBottomLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                        });
                    offset += 4;
                }

                // -Y Quad
                if (ny)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                        });
                    offset += 4;
                }

                // +Z Quad
                if (pz)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz + 0.5f}, TextureMap::GetTopLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz + 0.5f}, TextureMap::GetBottomRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                        });
                    offset += 4;
                }

                // -Z Quad
                if (nz)
                {
                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(
                        m_Vertices.end(),
                        {
                            Vertex({worldx + 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                            Vertex({worldx + 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                            Vertex({worldx - 0.5f, worldy - 0.5f, worldz - 0.5f}, TextureMap::GetBottomLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                            Vertex({worldx - 0.5f, worldy + 0.5f, worldz - 0.5f}, TextureMap::GetTopLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                        });
                    offset += 4;
                }
            }
        }
    }
}
