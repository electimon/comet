#include "Chunk.h"

#include "glm/gtc/random.hpp"

#include "FastNoiseLite.h"

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
    // glm::vec4 rgba;
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

        float pxpypz = 1.0f;
        float pxpynz = 1.0f;
        float pxnypz = 1.0f;
        float pxnynz = 1.0f;
        float nxpypz = 1.0f;
        float nxpynz = 1.0f;
        float nxnypz = 1.0f;
        float nxnynz = 1.0f;

        x = block.x + m_Chunk.x * m_ChunkSize;
        y = block.y;
        z = block.z + m_Chunk.z * m_ChunkSize;
        // glm::vec4 rgba = BlockColor[blockID];

        // +X Quad
        if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopRight(blockID), glm::vec3(+1.0f, 0.0f, 0.0f), pxpypz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(blockID), glm::vec3(+1.0f, 0.0f, 0.0f), pxnypz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(blockID), glm::vec3(+1.0f, 0.0f, 0.0f), pxnynz),
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopLeft(blockID), glm::vec3(+1.0f, 0.0f, 0.0f), pxpynz),
                              });
            m_Offset += 4;
        }

        // -X Quad
        if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopRight(blockID), glm::vec3(-1.0f, 0.0f, 0.0f), nxpypz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopLeft(blockID), glm::vec3(-1.0f, 0.0f, 0.0f), nxpynz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(blockID), glm::vec3(-1.0f, 0.0f, 0.0f), nxnynz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(blockID), glm::vec3(-1.0f, 0.0f, 0.0f), nxnypz),
                              });
            m_Offset += 4;
        }

        // +Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});

            glm::vec2 test = TextureMap::GetTopRight(blockID);

            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopLeft(blockID), glm::vec3(0.0f, +1.0f, 0.0f), pxpypz),
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopRight(blockID), glm::vec3(0.0f, +1.0f, 0.0f), pxpynz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetBottomRight(blockID), glm::vec3(0.0f, +1.0f, 0.0f), nxpynz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetBottomLeft(blockID), glm::vec3(0.0f, +1.0f, 0.0f), nxpypz),
                              });
            m_Offset += 4;
        }

        // -Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetTopRight(blockID), glm::vec3(0.0f, -1.0f, 0.0f), pxnypz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(blockID), glm::vec3(0.0f, -1.0f, 0.0f), nxnypz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(blockID), glm::vec3(0.0f, -1.0f, 0.0f), nxnynz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetTopLeft(blockID), glm::vec3(0.0f, -1.0f, 0.0f), pxnynz),
                              });
            m_Offset += 4;
        }

        // +Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z + 1)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopRight(blockID), glm::vec3(0.0f, 0.0f, +1.0f), pxpypz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopLeft(blockID), glm::vec3(0.0f, 0.0f, +1.0f), nxpypz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomLeft(blockID), glm::vec3(0.0f, 0.0f, +1.0f), nxnypz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(blockID), glm::vec3(0.0f, 0.0f, +1.0f), pxnypz),
                              });
            m_Offset += 4;
        }

        // -Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z - 1)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopRight(blockID), glm::vec3(0.0f, 0.0f, -1.0f), pxpynz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomRight(blockID), glm::vec3(0.0f, 0.0f, -1.0f), pxnynz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(blockID), glm::vec3(0.0f, 0.0f, -1.0f), nxnynz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopLeft(blockID), glm::vec3(0.0f, 0.0f, -1.0f), nxpynz),
                              });
            m_Offset += 4;
        }
    }
}

void Chunk::GenerateSurface()
{
    // can possibly use this for biomes
    FastNoiseLite cNoise;
    cNoise.SetFrequency(0.02f);
    cNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
    cNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Euclidean);
    cNoise.SetCellularReturnType(FastNoiseLite::CellularReturnType::CellularReturnType_Distance2Sub);
    float cNoiseValue;

    // mostly going to be used for terrain
    FastNoiseLite sNoise;
    sNoise.SetFrequency(0.01f);
    sNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
    float sNoiseValue;

    // can possibly be used for decorations
    FastNoiseLite wNoise;
    wNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
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
            cNoiseValue = cNoise.GetNoise((float)(x + m_ChunkSize * m_Chunk.x), float(z + m_ChunkSize * m_Chunk.z));
            sNoiseValue = sNoise.GetNoise((float)(x + m_ChunkSize * m_Chunk.x), (float)(z + m_ChunkSize * m_Chunk.z));
            // wNoiseValue = wNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z);

            // Calculating a surface height with the noise
            float height = cNoiseValue * sNoiseValue;

            height *= 32.0f; // scale
            height += 24.0f; // offset
            m_SurfaceHeights.insert_or_assign(glm::ivec2(x, z), (int)height);

            for (int y = 0; y < height - 3; y++)
            {
                SetBlock(glm::ivec3(x, y, z), Block(241)); // stone
            }

            SetBlock(glm::ivec3(x, height - 3, z), Block(242)); // dirt
            SetBlock(glm::ivec3(x, height - 2, z), Block(242)); // dirt
            SetBlock(glm::ivec3(x, height - 1, z), Block(242)); // dirt
            SetBlock(glm::ivec3(x, height, z), Block(240));     // grass
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

                SetBlock(glm::ivec3(xRel, yRel, zRel), Block(226));
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
                    SetBlock(glm::ivec3(xRel, yRel, zRel), Block(63)); // water
                    y = yRel;
                }
            }
        }
    }
}

void Chunk::GenerateTrees()
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
    float noiseValue;
    float noiseValue2;

    for (int x = 2; x < m_ChunkSize - 2; x++)
    {
        for (int z = 2; z < m_ChunkSize - 2; z++)
        {
            // 0 to 1 noise values
            noise.SetFrequency(2.0f);
            noiseValue = (noise.GetNoise((float)(x + m_ChunkSize * m_Chunk.x), (float)(z + m_ChunkSize * m_Chunk.z)) + 1.0f) / 2.0f;
            noise.SetFrequency(0.1f);
            noiseValue2 = (noise.GetNoise((float)(x + m_ChunkSize * m_Chunk.x), (float)(z + m_ChunkSize * m_Chunk.z)) + 1.0f) / 2.0f;

            if (noiseValue > 0.95f && noiseValue2 > 0.5f && m_SurfaceHeights.at(glm::ivec2(x, z)) > World::GetWaterHeight() + 3)
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

    SetBlock(glm::ivec3(x, y, z), Block(242));
    SetBlock(glm::ivec3(x, y + 1, z), Block(228));
    SetBlock(glm::ivec3(x, y + 2, z), Block(228));
    SetBlock(glm::ivec3(x, y + 3, z), Block(228));

    for (int i = x - 2; i < x + 3; i++)
    {
        for (int j = z - 2; j < z + 3; j++)
        {
            if ((i == x - 2 && j == z - 2) || (i == x + 2 && j == z + 2) || (i == x - 2 && j == z + 2) || (i == x + 2 && j == z - 2))
                continue;

            SetBlock(glm::ivec3(i, y + 5, j), Block(197)); // leaves
            SetBlock(glm::ivec3(i, y + 4, j), Block(197)); // leaves
        }
    }

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = z - 1; j < z + 2; j++)
        {
            SetBlock(glm::ivec3(i, y + 6, j), Block(197)); // leaves
        }
    }

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = z - 1; j < z + 2; j++)
        {
            if ((i == x - 1 && j == z - 1) || (i == x + 1 && j == z + 1) || (i == x - 1 && j == z + 1) || (i == x + 1 && j == z - 1))
                continue;

            SetBlock(glm::ivec3(i, y + 7, j), Block(197)); // leaves
        }
    }
}
