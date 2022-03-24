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
        bool use_shitting_lighting = false;

        glm::ivec3 block = index.first;
        unsigned int blockID = index.second.GetID();

        // Because blocks were technically generated outside the bounds of the chunk
        // to assist with the mesh generation, check the bounds here.
        if (block.x < 0 || block.y < 0 || block.z < 0)
            continue;
        if (block.x > m_ChunkSize || block.y > m_ChunkHeight || block.z > m_ChunkSize)
            continue;

        float pxpypz = 1.0f;
        float pxpynz = 1.0f;
        float pxnypz = 1.0f;
        float pxnynz = 1.0f;
        float nxpypz = 1.0f;
        float nxpynz = 1.0f;
        float nxnypz = 1.0f;
        float nxnynz = 1.0f;

        if (use_shitting_lighting)
        {

            // Corners
            bool pxpypz_Corner = m_Blocks.find(glm::ivec3(block.x + 1, block.y + 1, block.z + 1)) != m_Blocks.end();
            bool pxpynz_Corner = m_Blocks.find(glm::ivec3(block.x + 1, block.y + 1, block.z - 1)) != m_Blocks.end();
            bool pxnypz_Corner = m_Blocks.find(glm::ivec3(block.x + 1, block.y - 1, block.z + 1)) != m_Blocks.end();
            bool pxnynz_Corner = m_Blocks.find(glm::ivec3(block.x + 1, block.y - 1, block.z - 1)) != m_Blocks.end();
            bool nxpypz_Corner = m_Blocks.find(glm::ivec3(block.x - 1, block.y + 1, block.z + 1)) != m_Blocks.end();
            bool nxpynz_Corner = m_Blocks.find(glm::ivec3(block.x - 1, block.y + 1, block.z - 1)) != m_Blocks.end();
            bool nxnypz_Corner = m_Blocks.find(glm::ivec3(block.x - 1, block.y - 1, block.z + 1)) != m_Blocks.end();
            bool nxnynz_Corner = m_Blocks.find(glm::ivec3(block.x - 1, block.y - 1, block.z - 1)) != m_Blocks.end();

            // Edges
            bool pxpy_Edge = m_Blocks.find(glm::ivec3(block.x + 1, block.y + 1, block.z)) != m_Blocks.end();
            bool nxpy_Edge = m_Blocks.find(glm::ivec3(block.x - 1, block.y + 1, block.z)) != m_Blocks.end();
            bool pynz_Edge = m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z - 1)) != m_Blocks.end();
            bool pypz_Edge = m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z + 1)) != m_Blocks.end();
            bool pxpz_Edge = m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z + 1)) != m_Blocks.end();
            bool pxnz_Edge = m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z - 1)) != m_Blocks.end();
            bool nxpz_Edge = m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z + 1)) != m_Blocks.end();
            bool nxnz_Edge = m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z - 1)) != m_Blocks.end();
            bool pxny_Edge = m_Blocks.find(glm::ivec3(block.x + 1, block.y - 1, block.z)) != m_Blocks.end();
            bool nxny_Edge = m_Blocks.find(glm::ivec3(block.x - 1, block.y - 1, block.z)) != m_Blocks.end();
            bool nypz_Edge = m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z + 1)) != m_Blocks.end();
            bool nynz_Edge = m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z - 1)) != m_Blocks.end();

            float scale = 0.25f;

            if (pxpy_Edge)
            {
                pxpypz -= scale;
                pxpynz -= scale;
            }
            if (nxpy_Edge)
            {
                nxpypz -= scale;
                nxpynz -= scale;
            }
            if (pypz_Edge)
            {
                pxpypz -= scale;
                nxpypz -= scale;
            }
            if (pynz_Edge)
            {
                pxpynz -= scale;
                nxpynz -= scale;
            }
            if (pxpz_Edge)
            {
                pxpypz -= scale;
                pxnypz -= scale;
            }
            if (pxnz_Edge)
            {
                pxpynz -= scale;
                pxnynz -= scale;
            }
            if (nxpz_Edge)
            {
                nxpypz -= scale;
                nxnypz -= scale;
            }
            if (nxnz_Edge)
            {
                nxpynz -= scale;
                nxnynz -= scale;
            }
            if (pxny_Edge)
            {
                pxnypz -= scale;
                pxnynz -= scale;
            }
            if (nxny_Edge)
            {
                nxnypz -= scale;
                nxnynz -= scale;
            }
            if (nypz_Edge)
            {
                pxnypz -= scale;
                nxnypz -= scale;
            }
            if (nynz_Edge)
            {
                pxnynz -= scale;
                nxnynz -= scale;
            }

            if (pxpypz_Corner)
            {
                pxpypz -= scale;
            }
            if (pxpynz_Corner)
            {
                pxpynz -= scale;
            }
            if (pxnypz_Corner)
            {
                pxnypz -= scale;
            }
            if (pxnynz_Corner)
            {
                pxnynz -= scale;
            }
            if (nxpypz_Corner)
            {
                nxpypz -= scale;
            }
            if (nxpynz_Corner)
            {
                nxpynz -= scale;
            }
            if (nxnypz_Corner)
            {
                nxnypz -= scale;
            }
            if (nxnynz_Corner)
            {
                nxnynz -= scale;
            }
        }

        x = block.x + m_Chunk.x * m_ChunkSize;
        y = block.y;
        z = block.z + m_Chunk.z * m_ChunkSize;
        glm::vec4 rgba = BlockColor[blockID];

        // +X Quad
        if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba, +1.0f, 0.0f, 0.0f, pxpypz),
                                                    Vertex(x + 0.5f, y - 0.5f, z + 0.5f, rgba, +1.0f, 0.0f, 0.0f, pxnypz),
                                                    Vertex(x + 0.5f, y - 0.5f, z - 0.5f, rgba, +1.0f, 0.0f, 0.0f, pxnynz),
                                                    Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba, +1.0f, 0.0f, 0.0f, pxpynz),
                                                });
            m_Offset += 4;
        }

        // -X Quad
        if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba, -1.0f, 0.0f, 0.0f, nxpypz),
                                                    Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba, -1.0f, 0.0f, 0.0f, nxpynz),
                                                    Vertex(x - 0.5f, y - 0.5f, z - 0.5f, rgba, -1.0f, 0.0f, 0.0f, nxnynz),
                                                    Vertex(x - 0.5f, y - 0.5f, z + 0.5f, rgba, -1.0f, 0.0f, 0.0f, nxnypz),
                                                });
            m_Offset += 4;
        }

        // +Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, +1.0f, 0.0f, pxpypz),
                                                    Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, +1.0f, 0.0f, pxpynz),
                                                    Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, +1.0f, 0.0f, nxpynz),
                                                    Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, +1.0f, 0.0f, nxpypz),
                                                });
            m_Offset += 4;
        }

        // -Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, -1.0f, 0.0f, pxnypz),
                                                    Vertex(x - 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, -1.0f, 0.0f, nxnypz),
                                                    Vertex(x - 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, -1.0f, 0.0f, nxnynz),
                                                    Vertex(x + 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, -1.0f, 0.0f, pxnynz),
                                                });
            m_Offset += 4;
        }

        // +Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z + 1)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, pxpypz),
                                                    Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, nxpypz),
                                                    Vertex(x - 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, nxnypz),
                                                    Vertex(x + 0.5f, y - 0.5f, z + 0.5f, rgba, 0.0f, 0.0f, +1.0f, pxnypz),
                                                });
            m_Offset += 4;
        }

        // -Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z - 1)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(), {
                                                    Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, pxpynz),
                                                    Vertex(x + 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, pxnynz),
                                                    Vertex(x - 0.5f, y - 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, nxnynz),
                                                    Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba, 0.0f, 0.0f, -1.0f, nxpynz),
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
    int xOffset = m_Chunk.x * m_ChunkSize;
    int zOffset = m_Chunk.z * m_ChunkSize;

    for (int xRel = -1; xRel < m_ChunkSize + 2; xRel++)
    {
        for (int zRel = -1; zRel < m_ChunkSize + 2; zRel++)
        {
            if (m_SurfaceHeights.at(glm::ivec2(xRel, zRel)) < World::GetWaterHeight() + 3)
            {
                int yRel = m_SurfaceHeights.at(glm::ivec2(xRel, zRel));

                SetBlock(glm::ivec3(xRel, yRel, zRel), Block(7));

            }
        }
    }
}

void Chunk::GenerateWater()
{
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

            if (wNoiseValue > 0.995f && m_SurfaceHeights.at(glm::ivec2(x, z)) > World::GetWaterHeight() + 3)
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
