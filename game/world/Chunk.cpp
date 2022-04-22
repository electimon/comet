#include "Chunk.h"

#include "World.h"

Chunk::Chunk(glm::ivec3 id) : m_Chunk(id) {}

Chunk::~Chunk()
{
    if (m_Modified)
    {
        std::string filename = ".\\world\\" + std::to_string(m_Chunk.x) + " " + std::to_string(m_Chunk.z) + ".bin";
        std::ofstream os(filename, std::ios::binary);
        cereal::BinaryOutputArchive archive(os);
        archive(m_BlockData);
    }
}

void Chunk::Allocate()
{
    m_BlockData.fill(Block(0, true));
    m_HeightData.fill(0);

    m_SolidGeometry.Vertices.reserve(7000);
    m_SolidGeometry.Indices.reserve(7000);
    m_TransparentGeometry.Vertices.reserve(7000);
    m_TransparentGeometry.Indices.reserve(7000);
}

void Chunk::Generate()
{
    std::string filename = ".\\world\\" + std::to_string(m_Chunk.x) + " " + std::to_string(m_Chunk.z) + ".bin";
    if (std::filesystem::exists(filename))
    {
        std::cout << "Chunk file found, loading chunk...\n";
        std::ifstream is(filename, std::ios::binary);
        cereal::BinaryInputArchive archive(is);
        archive(m_BlockData);

        SetGenerated(false);
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

        SetGenerated(true);
    }
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

            height = 40.0f + (20.0f * noise1) + (10.0f * noise2) + (5.0f * noise3) + (2.5f * noise4);
            y = static_cast<int>(height);

            SetHeight(x, z, height);

            SetBlock({x, y, z}, Block(ID::Grass, false));
            SetBlock({x, y - 1, z}, Block(ID::Dirt, false));
            SetBlock({x, y - 2, z}, Block(ID::Dirt, false));
            SetBlock({x, y - 3, z}, Block(ID::Dirt, false));

            // fill chunk under dirt with stone
            for (int i = 0; i < y - 3; i++)
            {
                SetBlock({x, i, z}, Block(ID::Stone, false));
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

            // Check if generating a tree above water level
            if (y < water_height + 3 || water_height > mountain_height)
                continue;

            // Check to not generate a floating tree
            if (GetBlock({x, y - 1, z}).ID() == ID::Air)
                continue;

            noise1 = ChunkGenerator::GetFastNoise((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);
            noise2 = ChunkGenerator::GetMediumNoise((m_Chunk.x * CHUNK_WIDTH) + x, (m_Chunk.z * CHUNK_WIDTH) + z);

            if (noise1 > 0.9f && noise2 > 0.1f)
            {
                SetBlock({x, y, z}, Block(ID::Dirt, false));
                SetBlock({x, y + 1, z}, Block(ID::Oak_Log, false));
                SetBlock({x, y + 2, z}, Block(ID::Oak_Log, false));
                if (noise1 > 0.95f)
                {
                    SetBlock({x, y + 3, z}, Block(ID::Oak_Log, false));
                    y += 1;
                }
                SetBlock({x - 2, y + 3, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 2, y + 3, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 2, y + 3, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 3, z - 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 3, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 3, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 3, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 3, z + 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 3, z - 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 3, z - 1}, Block(ID::Oak_Leaves, true));

                SetBlock({x, y + 3, z}, Block(ID::Oak_Log, false));

                SetBlock({x, y + 3, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 3, z + 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 3, z - 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 3, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 3, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 3, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 3, z + 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 2, y + 3, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 2, y + 3, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 2, y + 3, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 2, y + 4, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 2, y + 4, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 2, y + 4, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 4, z - 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 4, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 4, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 4, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 4, z + 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 4, z - 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 4, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 4, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 4, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 4, z + 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 4, z - 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 4, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 4, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 4, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 4, z + 2}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 2, y + 4, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 2, y + 4, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 2, y + 4, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 5, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 5, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 5, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 5, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 5, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x - 1, y + 6, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 6, z - 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 6, z}, Block(ID::Oak_Leaves, true));
                SetBlock({x, y + 6, z + 1}, Block(ID::Oak_Leaves, true));
                SetBlock({x + 1, y + 6, z}, Block(ID::Oak_Leaves, true));
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

            SetBlock({x, 0, z}, Block(ID::Bedrock, false));

            if (noise > 0.20f)
                SetBlock({x, 1, z}, Block(ID::Bedrock, false));
            else
                continue;
            if (noise > 0.40f)
                SetBlock({x, 2, z}, Block(ID::Bedrock, false));
            else
                continue;
            if (noise > 0.60f)
                SetBlock({x, 3, z}, Block(ID::Bedrock, false));
            else
                continue;
            if (noise > 0.80f)
                SetBlock({x, 4, z}, Block(ID::Bedrock, false));
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
                if (GetBlock({x, y, z}).ID() != 1)
                    continue;

                noise = ChunkGenerator::GetCaveNoise(x + m_Chunk.x * CHUNK_WIDTH, y, z + m_Chunk.z * CHUNK_WIDTH);

                if (noise > 0.8f)
                    SetBlock({x, y, z}, Block(ID::Air, true));
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
                    SetBlock({x, i, z}, Block(ID::Water, true));
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
                SetBlock({x, y, z}, Block(ID::Sand, false));
                SetBlock({x, y - 1, z}, Block(ID::Sand, false));
                SetBlock({x, y - 2, z}, Block(ID::Sand, false));
            }

            if (y == WATER_HEIGHT + 4 && noise > 0.5f)
            {
                SetBlock({x, y, z}, Block(ID::Sand, false));
                SetBlock({x, y - 1, z}, Block(ID::Sand, false));
                SetBlock({x, y - 2, z}, Block(ID::Sand, false));
            }
        }
    }
}

void Chunk::GenerateMesh()
{
    m_SolidGeometry.Vertices.clear();
    m_SolidGeometry.Indices.clear();
    m_SolidGeometry.Offset = 0;

    m_TransparentGeometry.Vertices.clear();
    m_TransparentGeometry.Indices.clear();
    m_TransparentGeometry.Offset = 0;

    Block currentBlock;
    Block pxBlock, nxBlock, pyBlock, nyBlock, pzBlock, nzBlock;
    bool px, nx, py, ny, pz, nz;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_WIDTH; z++)
            {
                currentBlock = GetBlock({x, y, z});

                // If the block is air, add no geometry
                if (currentBlock.ID() == ID::Air)
                {
                    continue;
                }

                Geometry &geometry = currentBlock.IsTransparent() ? m_TransparentGeometry : m_SolidGeometry;
                unsigned int &offset =
                    currentBlock.IsTransparent() ? m_TransparentGeometry.Offset : m_SolidGeometry.Offset;

                // Get indices of texture for given block ID
                std::array<unsigned char, 6> blockIndices = BlockLibrary::GetIndices(currentBlock.ID());

                // Getting block IDs of surrounding blocks
                if (x == 0)
                {
                    pxBlock = GetBlock({x + 1, y, z});
                    nxBlock = World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH - 1, y, z + m_Chunk.z * CHUNK_WIDTH});
                }
                else
                {
                    if (x == CHUNK_WIDTH - 1)
                    {
                        pxBlock = World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH + 1, y, z + m_Chunk.z * CHUNK_WIDTH});
                        nxBlock = GetBlock({x - 1, y, z});
                    }
                    else
                    {
                        pxBlock = GetBlock({x + 1, y, z});
                        nxBlock = GetBlock({x - 1, y, z});
                    }
                }

                pyBlock = GetBlock({x, y + 1, z});
                nyBlock = GetBlock({x, y - 1, z});

                if (z == 0)
                {
                    pzBlock = GetBlock({x, y, z + 1});
                    nzBlock = World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH, y, z + m_Chunk.z * CHUNK_WIDTH - 1});
                }
                else
                {
                    if (z == CHUNK_WIDTH - 1)
                    {
                        pzBlock = World::GetBlock({x + m_Chunk.x * CHUNK_WIDTH, y, z + m_Chunk.z * CHUNK_WIDTH + 1});
                        nzBlock = GetBlock({x, y, z - 1});
                    }
                    else
                    {
                        pzBlock = GetBlock({x, y, z + 1});
                        nzBlock = GetBlock({x, y, z - 1});
                    }
                }

                // Determining if side should be rendered
                px = pxBlock.IsTransparent();
                nx = nxBlock.IsTransparent();
                py = pyBlock.IsTransparent();
                ny = nyBlock.IsTransparent();
                pz = pzBlock.IsTransparent();
                nz = nzBlock.IsTransparent();

                if (currentBlock.IsTransparent())
                {
                    px = true;
                    nx = true;
                    py = true;
                    ny = true;
                    pz = true;
                    nz = true;
                }

                // Water rendering
                if (currentBlock.ID() == ID::Water)
                {
                    px = pxBlock.ID() == ID::Water || !pxBlock.IsTransparent() ? false : true;
                    nx = nxBlock.ID() == ID::Water || !nxBlock.IsTransparent() ? false : true;
                    py = pyBlock.ID() == ID::Water || !pyBlock.IsTransparent() ? false : true;
                    ny = nyBlock.ID() == ID::Water || !nyBlock.IsTransparent() ? false : true;
                    pz = pzBlock.ID() == ID::Water || !pzBlock.IsTransparent() ? false : true;
                    nz = nzBlock.ID() == ID::Water || !nzBlock.IsTransparent() ? false : true;
                }

                // +X Quad
                if (px)
                {
                    geometry.Indices.insert(geometry.Indices.end(),
                                            {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    geometry.Vertices.insert(geometry.Vertices.end(),
                                             {
                                                 Vertex({x + 0.5f, y + 0.5f, z + 0.5f},
                                                        TextureMap::TopRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                                 Vertex({x + 0.5f, y - 0.5f, z + 0.5f},
                                                        TextureMap::BottomRight(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                                 Vertex({x + 0.5f, y - 0.5f, z - 0.5f},
                                                        TextureMap::BottomLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                                 Vertex({x + 0.5f, y + 0.5f, z - 0.5f},
                                                        TextureMap::TopLeft(blockIndices[0]), {+1.0f, 0.0f, 0.0f}),
                                             });
                    offset += 4;
                }

                // -X Quad
                if (nx)
                {
                    geometry.Indices.insert(geometry.Indices.end(),
                                            {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    geometry.Vertices.insert(geometry.Vertices.end(),
                                             {
                                                 Vertex({x - 0.5f, y + 0.5f, z + 0.5f},
                                                        TextureMap::TopRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                                 Vertex({x - 0.5f, y + 0.5f, z - 0.5f},
                                                        TextureMap::TopLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                                 Vertex({x - 0.5f, y - 0.5f, z - 0.5f},
                                                        TextureMap::BottomLeft(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                                 Vertex({x - 0.5f, y - 0.5f, z + 0.5f},
                                                        TextureMap::BottomRight(blockIndices[1]), {-1.0f, 0.0f, 0.0f}),
                                             });
                    offset += 4;
                }

                // +Y Quad
                if (py)
                {
                    geometry.Indices.insert(geometry.Indices.end(),
                                            {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    geometry.Vertices.insert(geometry.Vertices.end(),
                                             {
                                                 Vertex({x + 0.5f, y + 0.5f, z + 0.5f},
                                                        TextureMap::TopLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                                 Vertex({x + 0.5f, y + 0.5f, z - 0.5f},
                                                        TextureMap::TopRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                                 Vertex({x - 0.5f, y + 0.5f, z - 0.5f},
                                                        TextureMap::BottomRight(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                                 Vertex({x - 0.5f, y + 0.5f, z + 0.5f},
                                                        TextureMap::BottomLeft(blockIndices[2]), {0.0f, +1.0f, 0.0f}),
                                             });
                    offset += 4;
                }

                // -Y Quad
                if (ny)
                {
                    geometry.Indices.insert(geometry.Indices.end(),
                                            {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    geometry.Vertices.insert(geometry.Vertices.end(),
                                             {
                                                 Vertex({x + 0.5f, y - 0.5f, z + 0.5f},
                                                        TextureMap::TopRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                                 Vertex({x - 0.5f, y - 0.5f, z + 0.5f},
                                                        TextureMap::BottomRight(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                                 Vertex({x - 0.5f, y - 0.5f, z - 0.5f},
                                                        TextureMap::BottomLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                                 Vertex({x + 0.5f, y - 0.5f, z - 0.5f},
                                                        TextureMap::TopLeft(blockIndices[3]), {0.0f, -1.0f, 0.0f}),
                                             });
                    offset += 4;
                }

                // +Z Quad
                if (pz)
                {
                    geometry.Indices.insert(geometry.Indices.end(),
                                            {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    geometry.Vertices.insert(geometry.Vertices.end(),
                                             {
                                                 Vertex({x + 0.5f, y + 0.5f, z + 0.5f},
                                                        TextureMap::TopRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                                 Vertex({x - 0.5f, y + 0.5f, z + 0.5f},
                                                        TextureMap::TopLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                                 Vertex({x - 0.5f, y - 0.5f, z + 0.5f},
                                                        TextureMap::BottomLeft(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                                 Vertex({x + 0.5f, y - 0.5f, z + 0.5f},
                                                        TextureMap::BottomRight(blockIndices[4]), {0.0f, 0.0f, +1.0f}),
                                             });
                    offset += 4;
                }

                // -Z Quad
                if (nz)
                {
                    geometry.Indices.insert(geometry.Indices.end(),
                                            {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset});
                    geometry.Vertices.insert(geometry.Vertices.end(),
                                             {
                                                 Vertex({x + 0.5f, y + 0.5f, z - 0.5f},
                                                        TextureMap::TopRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                                 Vertex({x + 0.5f, y - 0.5f, z - 0.5f},
                                                        TextureMap::BottomRight(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                                 Vertex({x - 0.5f, y - 0.5f, z - 0.5f},
                                                        TextureMap::BottomLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                                 Vertex({x - 0.5f, y + 0.5f, z - 0.5f},
                                                        TextureMap::TopLeft(blockIndices[5]), {0.0f, 0.0f, -1.0f}),
                                             });
                    offset += 4;
                }
            }
        }
    }

    // std::cout << "Solid Vertices: " << m_SolidGeometry.Vertices.size() << "\n";
    // std::cout << "Solid Indices: " << m_SolidGeometry.Indices.size() << "\n";
    // std::cout << "Transparent Vertices: " << m_TransparentGeometry.Vertices.size() << "\n";
    // std::cout << "Transparent Indices: " << m_TransparentGeometry.Indices.size() << "\n\n";
}
