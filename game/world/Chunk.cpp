#include "Chunk.h"

#include "Renderer.h"
#include "render/TextureMap.h"
#include "render/Vertex.h"

#include "ChunkGenerator.h"
#include "Timer.h"
#include "World.h"
#include "world/BlockLibrary.h"
#include "world/WorldConfig.h"

Chunk::Chunk(glm::ivec3 id) : m_Chunk(id)
{
    m_BlockData.fill(0);
    m_HeightData.fill(0);

    m_Vertices.reserve(100000);
    m_Indices.reserve(100000);
}

Chunk::~Chunk()
{
    if (m_Modified)
    {
        std::ofstream blockDataFile(".\\world\\" + std::to_string(m_Chunk.x) + " " + std::to_string(m_Chunk.y) + " " +
                                    std::to_string(m_Chunk.z) + ".chunk");
        std::copy(m_BlockData.begin(), m_BlockData.end(), std::ostream_iterator<unsigned char>(blockDataFile, ""));
        blockDataFile.close();
    }
}

void Chunk::Generate()
{
    if (std::filesystem::exists(".\\world\\" + std::to_string(m_Chunk.x) + " " + std::to_string(m_Chunk.y) + " " +
                                std::to_string(m_Chunk.z) + ".chunk"))
    {
        std::cout << "Loading chunk, reading chunk from disk...\n";

        std::string filename = ".\\world\\" + std::to_string(m_Chunk.x) + " " + std::to_string(m_Chunk.y) + " " +
                               std::to_string(m_Chunk.z) + ".chunk";
        std::basic_ifstream<char> blockDataFile(filename.c_str());

        // This will need to be redone once a more optimized method of
        // saving chunks is made.
        // m_BlockData = std::vector<unsigned char>( (std::istreambuf_iterator<char>(blockDataFile)),
        // std::istreambuf_iterator<char>());
    }
    else
    {
        // World Generation
        GenerateSurface();
        GenerateBedrock();
        // GenerateCaves();
        GenerateTrees();
        GenerateSand();
        GenerateWater();
    }

    m_Generated = true;
}

void Chunk::GenerateSurface()
{
    float height = 0.0f;
    float noise1, noise2, noise3, noise4, noise5;
    int y = 0;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_WIDTH; z++)
        {
            // Calculating a surface height with the noise

            noise1 = ChunkGenerator::GetPerlin1((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            noise2 = ChunkGenerator::GetPerlin2((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            noise3 = ChunkGenerator::GetPerlin4((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            noise4 = ChunkGenerator::GetPerlin8((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            // noise5 = ChunkGenerator::GetPerlin16((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);

            height = 40.0f + (20.0f * noise1) + (10.0f * noise2) + (5.0f * noise3) + (2.5f * noise4);
            y = static_cast<int>(height);

            SetHeight(x, z, height);

            SetBlock(x, y, z, BlockID::Grass);    // grass
            SetBlock(x, y - 1, z, BlockID::Dirt); // dirt
            SetBlock(x, y - 2, z, BlockID::Dirt); // dirt
            SetBlock(x, y - 3, z, BlockID::Dirt); // dirt

            // fill chunk under dirt with stone
            for (int i = 0; i < y - 3; i++)
            {
                SetBlock(x, i, z, BlockID::Stone);
            }
        }
    }
}

void Chunk::GenerateTrees()
{
    float noise1;
    float noise2;
    int y;
    int water_height = 30;
    int mountain_height = 70;

    for (int x = 2; x < CHUNK_WIDTH - 2; x++)
    {
        for (int z = 2; z < CHUNK_WIDTH - 2; z++)
        {
            y = GetHeight(x, z);

            if (y < water_height + 3 || water_height > mountain_height)
                continue;

            if (GetBlock(x, y - 1, z) == BlockID::Air)
                continue;

            noise1 = ChunkGenerator::GetFastNoise((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            noise2 = ChunkGenerator::GetMediumNoise((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);

            if (noise1 > 0.9f && noise2 > 0.1f)
            {
                SetBlock(x, y, z, BlockID::Dirt);
                SetBlock(x, y + 1, z, BlockID::Log);
                SetBlock(x, y + 2, z, BlockID::Log);
                if (noise1 > 0.95f)
                {
                    SetBlock(x, y + 3, z, BlockID::Log);
                    y += 1;
                }
                {
                    SetBlock(x - 2, y + 3, z - 1, BlockID::Leaves);
                    SetBlock(x - 2, y + 3, z, BlockID::Leaves);
                    SetBlock(x - 2, y + 3, z + 1, BlockID::Leaves);

                    SetBlock(x - 1, y + 3, z - 2, BlockID::Leaves);
                    SetBlock(x - 1, y + 3, z - 1, BlockID::Leaves);
                    SetBlock(x - 1, y + 3, z, BlockID::Leaves);
                    SetBlock(x - 1, y + 3, z + 1, BlockID::Leaves);
                    SetBlock(x - 1, y + 3, z + 2, BlockID::Leaves);

                    SetBlock(x, y + 3, z - 2, BlockID::Leaves);
                    SetBlock(x, y + 3, z - 1, BlockID::Leaves);
                    SetBlock(x, y + 3, z, 5);
                    SetBlock(x, y + 3, z + 1, BlockID::Leaves);
                    SetBlock(x, y + 3, z + 2, BlockID::Leaves);

                    SetBlock(x + 1, y + 3, z - 2, BlockID::Leaves);
                    SetBlock(x + 1, y + 3, z - 1, BlockID::Leaves);
                    SetBlock(x + 1, y + 3, z, BlockID::Leaves);
                    SetBlock(x + 1, y + 3, z + 1, BlockID::Leaves);
                    SetBlock(x + 1, y + 3, z + 2, BlockID::Leaves);

                    SetBlock(x + 2, y + 3, z - 1, BlockID::Leaves);
                    SetBlock(x + 2, y + 3, z, BlockID::Leaves);
                    SetBlock(x + 2, y + 3, z + 1, BlockID::Leaves);
                }
                {
                    SetBlock(x - 2, y + 4, z - 1, BlockID::Leaves);
                    SetBlock(x - 2, y + 4, z, BlockID::Leaves);
                    SetBlock(x - 2, y + 4, z + 1, BlockID::Leaves);

                    SetBlock(x - 1, y + 4, z - 2, BlockID::Leaves);
                    SetBlock(x - 1, y + 4, z - 1, BlockID::Leaves);
                    SetBlock(x - 1, y + 4, z, BlockID::Leaves);
                    SetBlock(x - 1, y + 4, z + 1, BlockID::Leaves);
                    SetBlock(x - 1, y + 4, z + 2, BlockID::Leaves);

                    SetBlock(x, y + 4, z - 2, BlockID::Leaves);
                    SetBlock(x, y + 4, z - 1, BlockID::Leaves);
                    SetBlock(x, y + 4, z, BlockID::Leaves);
                    SetBlock(x, y + 4, z + 1, BlockID::Leaves);
                    SetBlock(x, y + 4, z + 2, BlockID::Leaves);

                    SetBlock(x + 1, y + 4, z - 2, BlockID::Leaves);
                    SetBlock(x + 1, y + 4, z - 1, BlockID::Leaves);
                    SetBlock(x + 1, y + 4, z, BlockID::Leaves);
                    SetBlock(x + 1, y + 4, z + 1, BlockID::Leaves);
                    SetBlock(x + 1, y + 4, z + 2, BlockID::Leaves);

                    SetBlock(x + 2, y + 4, z - 1, BlockID::Leaves);
                    SetBlock(x + 2, y + 4, z, BlockID::Leaves);
                    SetBlock(x + 2, y + 4, z + 1, BlockID::Leaves);
                }
                {
                    SetBlock(x - 1, y + 5, z, BlockID::Leaves);

                    SetBlock(x, y + 5, z - 1, BlockID::Leaves);
                    SetBlock(x, y + 5, z, BlockID::Leaves);
                    SetBlock(x, y + 5, z + 1, BlockID::Leaves);

                    SetBlock(x + 1, y + 5, z, BlockID::Leaves);
                }
                {
                    SetBlock(x - 1, y + 6, z, BlockID::Leaves);

                    SetBlock(x, y + 6, z - 1, BlockID::Leaves);
                    SetBlock(x, y + 6, z, BlockID::Leaves);
                    SetBlock(x, y + 6, z + 1, BlockID::Leaves);

                    SetBlock(x + 1, y + 6, z, BlockID::Leaves);
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

            SetBlock(x, 0, z, BlockID::Bedrock);

            if (noise > 0.20f)
                SetBlock(x, 1, z, BlockID::Bedrock);
            else
                continue;
            if (noise > 0.40f)
                SetBlock(x, 2, z, BlockID::Bedrock);
            else
                continue;
            if (noise > 0.60f)
                SetBlock(x, 3, z, BlockID::Bedrock);
            else
                continue;
            if (noise > 0.80f)
                SetBlock(x, 4, z, BlockID::Bedrock);
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
                    SetBlock(x, y, z, BlockID::Air);
            }
        }
    }
}

void Chunk::GenerateWater()
{
    int y;
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_WIDTH; z++)
        {
            y = GetHeight(x, z);
            if (y < WATER_HEIGHT + 1)
            {
                for (unsigned int i = y + 1; i < WATER_HEIGHT + 1; i++)
                {
                    SetBlock(x, i, z, BlockID::Water);
                }
            }
        }
    }
}

void Chunk::GenerateSand()
{
    int y;
    float noise;
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_WIDTH; z++)
        {
            y = GetHeight(x, z);
            noise = ChunkGenerator::GetBiomeNoise(x, z);
            if (y < WATER_HEIGHT + 4)
            {
                SetBlock(x, y, z, BlockID::Sand);
                SetBlock(x, y - 1, z, BlockID::Sand);
                SetBlock(x, y - 2, z, BlockID::Sand);
            }

            if (y == WATER_HEIGHT + 4 && noise > 0.5f)
            {
                SetBlock(x, y, z, BlockID::Sand);
                SetBlock(x, y - 1, z, BlockID::Sand);
                SetBlock(x, y - 2, z, BlockID::Sand);
            }
        }
    }
}

void Chunk::GenerateMesh()
{
    m_Vertices.clear();
    m_Indices.clear();
    unsigned int offset = 0;

    unsigned char currentBlock;

    bool px, nx, py, ny, pz, nz;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_WIDTH; z++)
            {
                currentBlock = GetBlock(x, y, z);

                // If the block is air, add no geometry
                if (currentBlock == BlockID::Air)
                {
                    continue;
                }

                // Get indices of texture for given block ID
                std::array<unsigned char, 6> blockIndices = BlockTextures::GetIndices(currentBlock);

                // Check if blocks are surrounding in current chunk
                px = GetBlock(x + 1, y, z) == BlockID::Air;
                nx = GetBlock(x - 1, y, z) == BlockID::Air;
                py = GetBlock(x, y + 1, z) == BlockID::Air;
                ny = GetBlock(x, y - 1, z) == BlockID::Air;
                pz = GetBlock(x, y, z + 1) == BlockID::Air;
                nz = GetBlock(x, y, z - 1) == BlockID::Air;

                // If on the edge of a chunk, check neighboring chunk for block
                if (x == 0)
                {
                    if (World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH - 1, y, z + m_Chunk.z * CHUNK_WIDTH}) !=
                        BlockID::Air)
                    {
                        nx = false;
                    }
                }
                if (x == CHUNK_WIDTH - 1)
                {
                    if (World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH + 1, y, z + m_Chunk.z * CHUNK_WIDTH}) !=
                        BlockID::Air)
                    {
                        px = false;
                    }
                }
                if (z == 0)
                {
                    if (World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH, y, z + m_Chunk.z * CHUNK_WIDTH - 1}) !=
                        BlockID::Air)
                    {
                        nz = false;
                    }
                }
                if (z == CHUNK_WIDTH - 1)
                {
                    if (World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH, y, z + m_Chunk.z * CHUNK_WIDTH + 1}) !=
                        BlockID::Air)
                    {
                        pz = false;
                    }
                }

                // Render all sides of leaves
                if (currentBlock == BlockID::Leaves)
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
                    m_Indices.insert(m_Indices.end(),
                                     {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(m_Vertices.end(),
                                      {
                                          Vertex({x + 0.5f, y + 0.5f, z + 0.5f},
                                                 TextureMap::GetTopRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                          Vertex({x + 0.5f, y - 0.5f, z + 0.5f},
                                                 TextureMap::GetBottomRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                          Vertex({x + 0.5f, y - 0.5f, z - 0.5f},
                                                 TextureMap::GetBottomLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                          Vertex({x + 0.5f, y + 0.5f, z - 0.5f},
                                                 TextureMap::GetTopLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                      });
                    offset += 4;
                }

                // -X Quad
                if (nx)
                {
                    m_Indices.insert(m_Indices.end(),
                                     {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(m_Vertices.end(),
                                      {
                                          Vertex({x - 0.5f, y + 0.5f, z + 0.5f},
                                                 TextureMap::GetTopRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                          Vertex({x - 0.5f, y + 0.5f, z - 0.5f},
                                                 TextureMap::GetTopLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                          Vertex({x - 0.5f, y - 0.5f, z - 0.5f},
                                                 TextureMap::GetBottomLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                          Vertex({x - 0.5f, y - 0.5f, z + 0.5f},
                                                 TextureMap::GetBottomRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                      });
                    offset += 4;
                }

                // +Y Quad
                if (py)
                {
                    m_Indices.insert(m_Indices.end(),
                                     {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(m_Vertices.end(),
                                      {
                                          Vertex({x + 0.5f, y + 0.5f, z + 0.5f},
                                                 TextureMap::GetTopLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                          Vertex({x + 0.5f, y + 0.5f, z - 0.5f},
                                                 TextureMap::GetTopRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                          Vertex({x - 0.5f, y + 0.5f, z - 0.5f},
                                                 TextureMap::GetBottomRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                          Vertex({x - 0.5f, y + 0.5f, z + 0.5f},
                                                 TextureMap::GetBottomLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                      });
                    offset += 4;
                }

                // -Y Quad
                if (ny)
                {
                    m_Indices.insert(m_Indices.end(),
                                     {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(m_Vertices.end(),
                                      {
                                          Vertex({x + 0.5f, y - 0.5f, z + 0.5f},
                                                 TextureMap::GetTopRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                          Vertex({x - 0.5f, y - 0.5f, z + 0.5f},
                                                 TextureMap::GetBottomRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                          Vertex({x - 0.5f, y - 0.5f, z - 0.5f},
                                                 TextureMap::GetBottomLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                          Vertex({x + 0.5f, y - 0.5f, z - 0.5f},
                                                 TextureMap::GetTopLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                      });
                    offset += 4;
                }

                // +Z Quad
                if (pz)
                {
                    m_Indices.insert(m_Indices.end(),
                                     {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(m_Vertices.end(),
                                      {
                                          Vertex({x + 0.5f, y + 0.5f, z + 0.5f},
                                                 TextureMap::GetTopRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                          Vertex({x - 0.5f, y + 0.5f, z + 0.5f},
                                                 TextureMap::GetTopLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                          Vertex({x - 0.5f, y - 0.5f, z + 0.5f},
                                                 TextureMap::GetBottomLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                          Vertex({x + 0.5f, y - 0.5f, z + 0.5f},
                                                 TextureMap::GetBottomRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                      });
                    offset += 4;
                }

                // -Z Quad
                if (nz)
                {
                    m_Indices.insert(m_Indices.end(),
                                     {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    m_Vertices.insert(m_Vertices.end(),
                                      {
                                          Vertex({x + 0.5f, y + 0.5f, z - 0.5f},
                                                 TextureMap::GetTopRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                          Vertex({x + 0.5f, y - 0.5f, z - 0.5f},
                                                 TextureMap::GetBottomRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                          Vertex({x - 0.5f, y - 0.5f, z - 0.5f},
                                                 TextureMap::GetBottomLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                          Vertex({x - 0.5f, y + 0.5f, z - 0.5f},
                                                 TextureMap::GetTopLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                      });
                    offset += 4;
                }
            }
        }
    }
}
