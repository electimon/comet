#include "Chunk.h"

#include "glm/gtc/random.hpp"

#include "FastNoise.h"

#include "Renderer.h"
#include "containers/Vertex.h"

#include "World.h"
#include "Timer.h"
#include "BlockProperties.h"

Chunk::Chunk(glm::ivec3 id)
    : m_Chunk(id),
      m_ChunkSize(World::GetChunkSize()),
      m_ChunkHeight(World::GetChunkHeight()),
      m_Offset(0)
{

    // std::cout << "Chunk::Chunk()" << std::endl;

    m_Blocks.reserve(m_ChunkSize * m_ChunkSize * m_ChunkHeight);
    m_SurfaceHeights.reserve(m_ChunkSize * m_ChunkSize);
    m_Vertices.reserve(100000);
    m_Indices.reserve(100000);

    // These values were way too high
    // m_Vertices.reserve(8 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);
    // m_Indices.reserve(36 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);

    GenerateSurface();
    GenerateSand();
    GenerateWater();
    GenerateTrees();

    GenerateMesh();

    // std::cout << "Chunk contains: " << m_Vertices.size() << " vertices, " << m_Indices.size() << " indices." << std::endl;
}

Chunk::~Chunk()
{
    // std::cout << "Chunk::~Chunk()" << std::endl;
}

void Chunk::SetBlock(const glm::ivec3 &coordinate, const Block &block)
{
    m_Blocks.insert_or_assign(coordinate, block);
}

void Chunk::RemoveBlock(const glm::ivec3 &coordinate)
{
    m_Blocks.erase(coordinate);
}

void Chunk::GenerateMesh()
{
    glm::vec4 rgba;
    int x, y, z, blockID;

    for (auto &index : m_Blocks)
    {
        glm::ivec3 block = index.first;
        unsigned int blockID = index.second.GetID();

        // Because blocks were technically generated outside the bounds of the chunk
        // to assist with the mesh generation, check the bounds here.
        if (block.x < 0 || block.y < 0 || block.z < 0)
            continue;
        if (block.x > m_ChunkSize || block.y > m_ChunkHeight || block.z > m_ChunkSize)
            continue;

        x = block.x + m_Chunk.x * m_ChunkSize;
        y = block.y;
        z = block.z + m_Chunk.z * m_ChunkSize;
        glm::vec4 rgba = BlockColor[blockID];

        // If no block is found next to the block, then render that side.

        // +X Quad
        if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z)) == m_Blocks.end())
        {
            float posYLight = 0.0f;
            float negYLight = 0.0f;
            float posZLight = 0.0f;
            float negZLight = 0.0f;

            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y + 1, block.z)) != m_Blocks.end())
            {
                posYLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y + 1, block.z)) != m_Blocks.end())
            {
                negYLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z + 1)) != m_Blocks.end())
            {
                posZLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z - 1)) != m_Blocks.end())
            {
                negZLight += 0.2f;
            }

            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba, +1.0f, 0.0f, 0.0f, 1.0f - posYLight - posZLight), // 0
                                                    Vertex(x + 0.5f, y - 0.5f, z + 0.5f, rgba, +1.0f, 0.0f, 0.0f, 1.0f - negYLight - posZLight), // 1
                                                    Vertex(x + 0.5f, y - 0.5f, z - 0.5f, rgba, +1.0f, 0.0f, 0.0f, 1.0f - negYLight - negZLight), // 2
                                                    Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba, +1.0f, 0.0f, 0.0f, 1.0f - posYLight - negZLight), // 3
                                                });
            m_Offset += 4;
        }

        // -X Quad
        if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z)) == m_Blocks.end())
        {
            float posYLight = 0.0f;
            float negYLight = 0.0f;
            float posZLight = 0.0f;
            float negZLight = 0.0f;

            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y + 1, block.z)) != m_Blocks.end())
            {
                posYLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y + 1, block.z)) != m_Blocks.end())
            {
                negYLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z + 1)) != m_Blocks.end())
            {
                posZLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z - 1)) != m_Blocks.end())
            {
                negZLight += 0.2f;
            }

            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba, -1.0f, 0.0f, 0.0f, 1.0f - posYLight - posZLight), // 4
                                                    Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba, -1.0f, 0.0f, 0.0f, 1.0f - posYLight - negZLight), // 5
                                                    Vertex(x - 0.5f, y - 0.5f, z - 0.5f, rgba, -1.0f, 0.0f, 0.0f, 1.0f - negYLight - negZLight), // 6
                                                    Vertex(x - 0.5f, y - 0.5f, z + 0.5f, rgba, -1.0f, 0.0f, 0.0f, 1.0f - negYLight - posZLight), // 7
                                                });
            m_Offset += 4;
        }

        // +Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z)) == m_Blocks.end())
        {
            float posXLight = 0.0f;
            float negXLight = 0.0f;
            float posZLight = 0.0f;
            float negZLight = 0.0f;
            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y + 1, block.z)) != m_Blocks.end())
            {
                posXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y + 1, block.z)) != m_Blocks.end())
            {
                negXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z + 1)) != m_Blocks.end())
            {
                posZLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z - 1)) != m_Blocks.end())
            {
                negZLight += 0.2f;
            }

            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, +1.0f, 0.0f, 1.0f - posXLight - posZLight),
                                                    Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, +1.0f, 0.0f, 1.0f - posXLight - negZLight),
                                                    Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, +1.0f, 0.0f, 1.0f - negXLight - negZLight),
                                                    Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, +1.0f, 0.0f, 1.0f - negXLight - posZLight),
                                                });
            m_Offset += 4;
        }

        // -Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z)) == m_Blocks.end())
        {
            float posXLight = 0.0f;
            float negXLight = 0.0f;
            float posZLight = 0.0f;
            float negZLight = 0.0f;
            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y - 1, block.z)) != m_Blocks.end())
            {
                posXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y - 1, block.z)) != m_Blocks.end())
            {
                negXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z + 1)) != m_Blocks.end())
            {
                posZLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z - 1)) != m_Blocks.end())
            {
                negZLight += 0.2f;
            }

            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, -1.0f, 0.0f, 1.0f - posXLight - posZLight), // 12
                                                    Vertex(x - 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, -1.0f, 0.0f, 1.0f - negXLight - posZLight), // 13
                                                    Vertex(x - 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, -1.0f, 0.0f, 1.0f - negXLight - negZLight), // 14
                                                    Vertex(x + 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, -1.0f, 0.0f, 1.0f - posXLight - negZLight), // 15
                                                });
            m_Offset += 4;
        }

        // +Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z + 1)) == m_Blocks.end())
        {
            float posXLight = 0.0f;
            float negXLight = 0.0f;
            float posYLight = 0.0f;
            float negYLight = 0.0f;

            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z + 1)) != m_Blocks.end())
            {
                posXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z + 1)) != m_Blocks.end())
            {
                negXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z + 1)) != m_Blocks.end())
            {
                posYLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z + 1)) != m_Blocks.end())
            {
                negYLight += 0.2f;
            }

            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, 1.0f - posXLight - posYLight), // 16
                                                    Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, 1.0f - negXLight - posYLight), // 17
                                                    Vertex(x - 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, 1.0f - negXLight - negYLight), // 18
                                                    Vertex(x + 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, 1.0f - posXLight - negYLight), // 19
                                                });
            m_Offset += 4;
        }

        // -Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z - 1)) == m_Blocks.end())
        {
            float posXLight = 0.0f;
            float negXLight = 0.0f;
            float posYLight = 0.0f;
            float negYLight = 0.0f;

            if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z - 1)) != m_Blocks.end())
            {
                posXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z - 1)) != m_Blocks.end())
            {
                negXLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z - 1)) != m_Blocks.end())
            {
                posYLight += 0.2f;
            }
            if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z - 1)) != m_Blocks.end())
            {
                negYLight += 0.2f;
            }

            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, 1.0f - posXLight - posYLight), // 20
                                                    Vertex(x + 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, 1.0f - posXLight - negYLight), // 21
                                                    Vertex(x - 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, 1.0f - negXLight - negYLight), // 22
                                                    Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, 1.0f - negXLight - posYLight), // 23
                                                });
            m_Offset += 4;
        }
    }
}

void Chunk::GenerateSurface()
{
    // can possibly use this for biomes
    FastNoise cNoise;
    cNoise.SetFrequency(0.02);
    cNoise.SetNoiseType(FastNoise::NoiseType::Cellular);
    cNoise.SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Euclidean);
    cNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Sub);
    float cNoiseValue;

    // mostly going to be used for terrain
    FastNoise sNoise;
    sNoise.SetFrequency(0.01);
    sNoise.SetNoiseType(FastNoise::NoiseType::Simplex);
    float sNoiseValue;

    // can possibly be used for decorations
    FastNoise wNoise;
    wNoise.SetNoiseType(FastNoise::NoiseType::WhiteNoise);
    float wNoiseValue;

    // Increasing the range of surface generation to go one beyond the chunk size
    // makes it so that when generating the geometry for the chunk, it is able to
    // check for surrounding blocks on the edges of chunks correctly and doesn't push
    // indices between chunks to the GPU.
    for (int x = -1; x < m_ChunkSize + 2; x++)
    {
        for (int z = -1; z < m_ChunkSize + 2; z++)
        {
            // -1 to 1 noise values
            cNoiseValue = cNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z);
            sNoiseValue = sNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z);
            // wNoiseValue = wNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z);

            // Calculating a surface height with the noise
            float height = cNoiseValue * sNoiseValue;

            height *= 32.0f; // scale
            height += 24.0f; // offset
            m_SurfaceHeights.insert_or_assign(glm::ivec2(x, z), (int)height);

            for (int y = 0; y < height - 3; y++)
            {
                SetBlock(glm::ivec3(x, y, z), Block(1)); // stone
            }

            SetBlock(glm::ivec3(x, height - 3, z), Block(3)); // dirt
            SetBlock(glm::ivec3(x, height - 2, z), Block(3)); // dirt
            SetBlock(glm::ivec3(x, height - 1, z), Block(3)); // dirt
            SetBlock(glm::ivec3(x, height, z), Block(2));     // grass
        }
    }
}

void Chunk::GenerateSand()
{
}

void Chunk::GenerateWater()
{
    glm::vec4 rgba = glm::vec4(0.0f / 255.0f, 94.0f / 255.0f, 217.0f / 255.0f, 0.5f);
    int xOffset = m_Chunk.x * m_ChunkSize;
    int zOffset = m_Chunk.z * m_ChunkSize;

    for (int xRel = -1; xRel < m_ChunkSize + 2; xRel++)
    {
        for (int zRel = -1; zRel < m_ChunkSize + 2; zRel++)
        {
            if (m_SurfaceHeights.at(glm::ivec2(xRel, zRel)) < World::GetWaterHeight())
            {
                int x = xRel + xOffset;
                int y;
                int z = zRel + zOffset;

                // fill in with water surface level is below the world water height
                for (int yRel = m_SurfaceHeights.at(glm::ivec2(xRel, zRel)) + 1; yRel < World::GetWaterHeight() + 1; yRel++)
                {
                    SetBlock(glm::ivec3(xRel, yRel, zRel), Block(4)); // water
                    y = yRel;
                }

                // Drawing water here since its the easiest place to.
                // The coordinates are already set here in the for loop variables.
                // Only rendering the top of the water block.
                // TODO: add a rendering overlay when the camera is in a block of water.
                // m_Indices.insert(m_Indices.end(), {3 + m_Offset, 2 + m_Offset, 6 + m_Offset, 6 + m_Offset, 7 + m_Offset, 3 + m_Offset}); // top

                // m_Vertices.insert(m_Vertices.end(), {Vertex(x - 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                //                                      Vertex(x + 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                //                                      Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba),
                //                                      Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba),
                //                                      Vertex(x - 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                //                                      Vertex(x + 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                //                                      Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba),
                //                                      Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba)});

                // Offset should be correct after drawing regular terrain.
                // m_Offset += 8;
            }
        }
    }
}

void Chunk::GenerateTrees()
{
    FastNoise wNoise;
    wNoise.SetNoiseType(FastNoise::NoiseType::WhiteNoise);
    float wNoiseValue;

    for (int x = 2; x < m_ChunkSize - 2; x++)
    {
        for (int z = 2; z < m_ChunkSize - 2; z++)
        {
            // 0 to 1 noise values
            wNoiseValue = (wNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z) + 1.0f) / 2.0f;

            if (wNoiseValue > 0.995f && m_SurfaceHeights.at(glm::ivec2(x, z)) > World::GetWaterHeight())
            {
                PlaceTree(glm::ivec3(x, m_SurfaceHeights.at(glm::ivec2(x, z)), z));
            }
        }
    }
}

void Chunk::PlaceTree(const glm::ivec3 &coordinate)
{
    int x = coordinate.x;
    int y = coordinate.y;
    int z = coordinate.z;

    SetBlock(glm::ivec3(x, y, z), Block(3));
    SetBlock(glm::ivec3(x, y + 1, z), Block(5));
    SetBlock(glm::ivec3(x, y + 2, z), Block(5));
    SetBlock(glm::ivec3(x, y + 3, z), Block(5));

    for (int i = x - 2; i < x + 3; i++)
    {
        for (int j = z - 2; j < z + 3; j++)
        {
            if ((i == x - 2 && j == z - 2) || (i == x + 2 && j == z + 2) || (i == x - 2 && j == z + 2) || (i == x + 2 && j == z - 2))
                continue;

            SetBlock(glm::ivec3(i, y + 4, j), Block(6));
            SetBlock(glm::ivec3(i, y + 5, j), Block(6));
        }
    }

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = z - 1; j < z + 2; j++)
        {
            SetBlock(glm::ivec3(i, y + 6, j), Block(6));
        }
    }

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = z - 1; j < z + 2; j++)
        {
            if ((i == x - 1 && j == z - 1) || (i == x + 1 && j == z + 1) || (i == x - 1 && j == z + 1) || (i == x + 1 && j == z - 1))
                continue;

            SetBlock(glm::ivec3(i, y + 7, j), Block(6));
        }
    }
}
