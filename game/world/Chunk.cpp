#include "Chunk.h"

#include "glm/gtc/random.hpp"

#include "FastNoise.h"

#include "Renderer.h"
#include "containers/Vertex.h"

#include "World.h"

Chunk::Chunk(glm::ivec3 id)
    : m_Chunk(id),
      m_ChunkSize(World::GetChunkSize()),
      m_ChunkHeight(World::GetChunkHeight()),
      m_Offset(0)
{
    std::cout << "Chunk::Chunk()" << std::endl;
    m_Blocks.reserve(m_ChunkSize * m_ChunkSize);
    m_Vertices.reserve(8 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);
    m_Indices.reserve(36 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);

    GenerateSurface();
    GenerateWater();
    GenerateTrees();

    GenerateMesh();
    m_Mesh.AddGemoetry(m_Vertices, m_Indices);

    // GenerateMesh();
}

Chunk::~Chunk()
{
    std::cout << "Chunk::~Chunk()" << std::endl;
}

void Chunk::SetBlock(const glm::ivec3 &coordinate, const Block &block)
{
    m_Blocks.insert_or_assign(coordinate, block);
}

void Chunk::SetBlock(int x, int y, int z, const Block &block)
{
    m_Blocks.insert_or_assign(glm::ivec3(x, y, z), block);
}

void Chunk::RemoveBlock(int x, int y, int z)
{
    m_Blocks.erase(glm::ivec3(x, y, z));
}

void Chunk::RemoveBlock(const glm::ivec3 &coordinate)
{
    m_Blocks.erase(coordinate);
}

void Chunk::GenerateMesh()
{
    glm::vec4 rgba;
    int xOffset = m_Chunk.x * m_ChunkSize;
    int zOffset = m_Chunk.z * m_ChunkSize;

    int x, y, z, blockID;

    for (auto &block : m_Blocks)
    {
        // Because blocks were technically generated outside the bounds of the chunk
        // to assist with the mesh generation, check the bounds here.
        if (block.first.x < 0 || block.first.y < 0 || block.first.z < 0)
            continue;
        if (block.first.x > m_ChunkSize || block.first.y > m_ChunkHeight || block.first.z > m_ChunkSize)
            continue;


        x = block.first.x + xOffset;
        y = block.first.y;
        z = block.first.z + zOffset;
        blockID = block.second.GetID();


        // White block
        if (blockID == 0)
            rgba = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        // Stone
        if (blockID == 1)
            rgba = glm::vec4(94.0f / 255.0f, 94.0f / 255.0f, 94.0f / 255.0f, 1.0f);
        // Grass
        if (blockID == 2)
            rgba = glm::vec4(26.0f / 255.0f, 166.0f / 255.0f, 18.0f / 255.0f, 1.0f);
        // Dirt
        if (blockID == 3)
            rgba = glm::vec4(92.0f / 255.0f, 68.0f / 255.0f, 46.0f / 255.0f, 1.0f);
        // Water
        if (blockID == 4)
            rgba = glm::vec4(66.0f / 255.0f, 173.0f / 255.0f, 245.0f / 255.0f, 0.5f);
        // Log
        if (blockID == 5)
            rgba = glm::vec4(110.0f / 255.0f, 78.0f / 255.0f, 48.0f / 255.0f, 1.0f);
        // Leaves
        if (blockID == 6)
            rgba = glm::vec4(42.0f / 255.0f, 117.0f / 255.0f, 9.0f / 255.0f, 1.0f);


        // If no block is found next to the block, then render that side.
        if (m_Blocks.find(glm::ivec3(block.first.x, block.first.y, block.first.z + 1)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset}); // front
        }
        if (m_Blocks.find(glm::ivec3(block.first.x + 1, block.first.y, block.first.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {1 + m_Offset, 5 + m_Offset, 6 + m_Offset, 6 + m_Offset, 2 + m_Offset, 1 + m_Offset}); // right
        }
        if (m_Blocks.find(glm::ivec3(block.first.x, block.first.y, block.first.z - 1)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {5 + m_Offset, 4 + m_Offset, 7 + m_Offset, 7 + m_Offset, 6 + m_Offset, 5 + m_Offset}); // back
        }
        if (m_Blocks.find(glm::ivec3(block.first.x - 1, block.first.y, block.first.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {4 + m_Offset, 0 + m_Offset, 3 + m_Offset, 3 + m_Offset, 7 + m_Offset, 4 + m_Offset}); // left
        }
        if (m_Blocks.find(glm::ivec3(block.first.x, block.first.y - 1, block.first.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 4 + m_Offset, 5 + m_Offset, 5 + m_Offset, 1 + m_Offset, 0 + m_Offset}); // bottom
        }
        if (m_Blocks.find(glm::ivec3(block.first.x, block.first.y + 1, block.first.z)) == m_Blocks.end())
        {
            m_Indices.insert(m_Indices.end(), {3 + m_Offset, 2 + m_Offset, 6 + m_Offset, 6 + m_Offset, 7 + m_Offset, 3 + m_Offset}); // top
        }

        m_Vertices.insert(m_Vertices.end(), {Vertex(x - 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                             Vertex(x + 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                             Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba),
                                             Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba),
                                             Vertex(x - 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                             Vertex(x + 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                             Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba),
                                             Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba)});

        m_Offset += 8;
    }

    // WHY THE HELL WHY AM I LOOPING OVER THE BLOCKS LIKE THIS AHHHHHHHHHHHHHHHHHHHH

    // for (int xRel = 0; xRel < m_ChunkSize; xRel++)
    // {
    //     for (int yRel = 0; yRel < m_ChunkHeight; yRel++)
    //     {
    //         for (int zRel = 0; zRel < m_ChunkSize; zRel++)
    //         {
    //             int x = xRel + xOffset;
    //             int y = yRel;
    //             int z = zRel + zOffset;

    //             if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel)) == m_Blocks.end())
    //             {
    //                 continue;
    //             }

    //             // White block
    //             if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 0)
    //                 rgba = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //             // Stone
    //             if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 1)
    //                 rgba = glm::vec4(94.0f / 255.0f, 94.0f / 255.0f, 94.0f / 255.0f, 1.0f);
    //             // Grass
    //             if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 2)
    //                 rgba = glm::vec4(26.0f / 255.0f, 166.0f / 255.0f, 18.0f / 255.0f, 1.0f);
    //             // Dirt
    //             if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 3)
    //                 rgba = glm::vec4(92.0f / 255.0f, 68.0f / 255.0f, 46.0f / 255.0f, 1.0f);
    //             // Water
    //             if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 4)
    //                 rgba = glm::vec4(66.0f / 255.0f, 173.0f / 255.0f, 245.0f / 255.0f, 0.5f);
    //             // Log
    //             if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 5)
    //                 rgba = glm::vec4(110.0f / 255.0f, 78.0f / 255.0f, 48.0f / 255.0f, 1.0f);
    //             // Leaves
    //             if (m_Blocks.at(glm::ivec3(xRel, yRel, zRel)).GetID() == 6)
    //                 rgba = glm::vec4(42.0f / 255.0f, 117.0f / 255.0f, 9.0f / 255.0f, 1.0f);

    //             // If no block is found next to the block, then render that side.
    //             if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel + 1)) == m_Blocks.end())
    //             {
    //                 m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset}); // front
    //             }
    //             if (m_Blocks.find(glm::ivec3(xRel + 1, yRel, zRel)) == m_Blocks.end())
    //             {
    //                 m_Indices.insert(m_Indices.end(), {1 + m_Offset, 5 + m_Offset, 6 + m_Offset, 6 + m_Offset, 2 + m_Offset, 1 + m_Offset}); // right
    //             }
    //             if (m_Blocks.find(glm::ivec3(xRel, yRel, zRel - 1)) == m_Blocks.end())
    //             {
    //                 m_Indices.insert(m_Indices.end(), {5 + m_Offset, 4 + m_Offset, 7 + m_Offset, 7 + m_Offset, 6 + m_Offset, 5 + m_Offset}); // back
    //             }
    //             if (m_Blocks.find(glm::ivec3(xRel - 1, yRel, zRel)) == m_Blocks.end())
    //             {
    //                 m_Indices.insert(m_Indices.end(), {4 + m_Offset, 0 + m_Offset, 3 + m_Offset, 3 + m_Offset, 7 + m_Offset, 4 + m_Offset}); // left
    //             }
    //             if (m_Blocks.find(glm::ivec3(xRel, yRel - 1, zRel)) == m_Blocks.end())
    //             {
    //                 m_Indices.insert(m_Indices.end(), {0 + m_Offset, 4 + m_Offset, 5 + m_Offset, 5 + m_Offset, 1 + m_Offset, 0 + m_Offset}); // bottom
    //             }
    //             if (m_Blocks.find(glm::ivec3(xRel, yRel + 1, zRel)) == m_Blocks.end())
    //             {
    //                 m_Indices.insert(m_Indices.end(), {3 + m_Offset, 2 + m_Offset, 6 + m_Offset, 6 + m_Offset, 7 + m_Offset, 3 + m_Offset}); // top
    //             }

    //             m_Vertices.insert(m_Vertices.end(), {Vertex(x - 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
    //                                                  Vertex(x + 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
    //                                                  Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba),
    //                                                  Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba),
    //                                                  Vertex(x - 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
    //                                                  Vertex(x + 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
    //                                                  Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba),
    //                                                  Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba)});

    //             m_Offset += 8;
    //         }
    //     }
    // }

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
            // 0 to 1 noise values
            cNoiseValue = (cNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z) + 1.0f) / 2.0f;
            sNoiseValue = (sNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z) + 1.0f) / 2.0f;
            wNoiseValue = (wNoise.GetNoise(x + m_ChunkSize * m_Chunk.x, z + m_ChunkSize * m_Chunk.z) + 1.0f) / 2.0f;

            // Calculating a surface height with the noise
            float height = cNoiseValue * sNoiseValue;

            height *= float(World::GetChunkHeight() - 16); // scale
            m_SurfaceHeights.insert_or_assign(glm::ivec2(x, z), (int)height);

            for (int y = 0; y < height - 3; y++)
            {
                SetBlock(glm::ivec3(x, y, z), Block(1)); // stone
            }

            SetBlock(x, height - 3, z, Block(3)); // dirt
            SetBlock(x, height - 2, z, Block(3)); // dirt
            SetBlock(x, height - 1, z, Block(3)); // dirt
            SetBlock(x, height, z, Block(2));     // grass
        }
    }
}

void Chunk::GenerateWater()
{
    glm::vec4 rgba = glm::vec4(0.0f / 255.0f, 94.0f / 255.0f, 217.0f / 255.0f, 0.5f);
    int xOffset = m_Chunk.x * m_ChunkSize;
    int zOffset = m_Chunk.z * m_ChunkSize;

    for (int xRel = 0; xRel < m_ChunkSize; xRel++)
    {
        for (int zRel = 0; zRel < m_ChunkSize; zRel++)
        {
            if (m_SurfaceHeights.at(glm::ivec2(xRel, zRel)) < World::GetWaterHeight())
            {
                int x = xRel + xOffset;
                int y;
                int z = zRel + zOffset;

                // fill in with water surface level is below the world water height
                for (int yRel = m_SurfaceHeights.at(glm::ivec2(xRel, zRel)); yRel < World::GetWaterHeight(); yRel++)
                {
                    SetBlock(xRel, yRel, zRel, Block(4)); // water
                    y = yRel;
                }

                // Drawing water here since its the easiest place to
                // The coordinates are already set here in the for loop variables
                m_Indices.insert(m_Indices.end(), {3 + m_Offset, 2 + m_Offset, 6 + m_Offset, 6 + m_Offset, 7 + m_Offset, 3 + m_Offset}); // top

                m_Vertices.insert(m_Vertices.end(), {Vertex(x - 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                                     Vertex(x + 0.5f, y - 0.5f, z + 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                                     Vertex(x + 0.5f, y + 0.5f, z + 0.5f, rgba),
                                                     Vertex(x - 0.5f, y + 0.5f, z + 0.5f, rgba),
                                                     Vertex(x - 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                                     Vertex(x + 0.5f, y - 0.5f, z - 0.5f, 0.75f * rgba.r, 0.75f * rgba.g, 0.75f * rgba.b, rgba.a),
                                                     Vertex(x + 0.5f, y + 0.5f, z - 0.5f, rgba),
                                                     Vertex(x - 0.5f, y + 0.5f, z - 0.5f, rgba)});

                // Offset should be correct after drawing regular terrain.
                m_Offset += 8;
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
                PlaceTree(x, m_SurfaceHeights.at(glm::ivec2(x, z)), z);
            }
        }
    }
}

void Chunk::PlaceTree(int x, int y, int z)
{
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
            SetBlock(glm::ivec3(i, y + 6, j), Block(6)); // test block
        }
    }

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = z - 1; j < z + 2; j++)
        {
            if ((i == x - 1 && j == z - 1) || (i == x + 1 && j == z + 1) || (i == x - 1 && j == z + 1) || (i == x + 1 && j == z - 1))
                continue;

            SetBlock(glm::ivec3(i, y + 7, j), Block(6)); // test block
        }
    }
}
