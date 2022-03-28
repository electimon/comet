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
    m_Blocks.reserve(m_ChunkSize * m_ChunkSize * m_ChunkHeight);
    m_BlockData.reserve((m_ChunkSize + 1) * (m_ChunkSize + 1) * m_ChunkHeight); // "3D" array

    m_SurfaceHeights.reserve((m_ChunkSize) * (m_ChunkSize));
    m_SurfaceHeightData.reserve((m_ChunkSize) * (m_ChunkSize)); // "2D" array

    m_Vertices.reserve(10000);
    m_Indices.reserve(10000);

    GenerateSurface();

    // glm::ivec2 coordinate = {2, 3};

    // float test1 = m_SurfaceHeights.at(coordinate);
    // float test2 = GetHeight(coordinate);

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
    int i = 0;

    // 0 -> 15
    // -1 -> 16
    for (int x = 0; x < m_ChunkSize; x++)
    {
        for (int z = 0; z < m_ChunkSize; z++)
        {
            // Calculating a surface height with the noise
            height = ChunkGenerator::GetSurfaceNoise(static_cast<int>(m_Chunk.x * m_ChunkSize) + x, static_cast<int>(m_Chunk.z * m_ChunkSize) + z);
            biome = ChunkGenerator::GetBiomeNoise(static_cast<int>(m_Chunk.x * m_ChunkSize) + x, static_cast<int>(m_Chunk.z * m_ChunkSize) + z);

            // height += 1.0f; // 0 to 2
            biome += 1.0f;  // 0 to 2
            height *= 2.5f; // 0 10
            biome *= 50.0f; // 0 10
            float y = static_cast<int>(height + biome);

            m_SurfaceHeights.insert_or_assign(glm::ivec2(x, z), y);
            m_SurfaceHeightData.push_back(y);

            for (int i = 0; i < 50; i++)
            {
                m_Blocks.insert_or_assign(glm::ivec3(x, static_cast<int>(height + biome), z), Block(2));
                // m_BlockData.set(1.0f);
                SetBlock({x, y, z}, 1.0f); // updates block data
            }
        }
    }
}

void Chunk::GenerateMesh()
{
    int x, y, z, blockID;

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

    for (auto &index : m_Blocks)
    {
        glm::ivec3 block = index.first;
        unsigned int blockID = index.second.GetID();

        // Because blocks were technically generated outside the bounds of the chunk
        // to assist with the mesh generation, check the bounds here.
        if (block.x < 0 || block.y < 0 || block.z < 0)
            continue;
        if (block.x == m_ChunkSize || block.y == m_ChunkHeight || block.z == m_ChunkSize)
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

        // +X Quad
        // if (m_Blocks.find(glm::ivec3(block.x + 1, block.y, block.z)) == m_Blocks.end())
        if (GetBlock({x + 1, y, z}))
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopRight(textureIndices[blockID - 1][0]), glm::vec3(+1.0f, 0.0f, 0.0f), pxpypz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(textureIndices[blockID - 1][0]), glm::vec3(+1.0f, 0.0f, 0.0f), pxnypz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(textureIndices[blockID - 1][0]), glm::vec3(+1.0f, 0.0f, 0.0f), pxnynz),
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopLeft(textureIndices[blockID - 1][0]), glm::vec3(+1.0f, 0.0f, 0.0f), pxpynz),
                              });
            m_Offset += 4;
        }

        // -X Quad
        // if (m_Blocks.find(glm::ivec3(block.x - 1, block.y, block.z)) == m_Blocks.end())
        if (GetBlock({x - 1, y, z}))
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopRight(textureIndices[blockID - 1][1]), glm::vec3(-1.0f, 0.0f, 0.0f), nxpypz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopLeft(textureIndices[blockID - 1][1]), glm::vec3(-1.0f, 0.0f, 0.0f), nxpynz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(textureIndices[blockID - 1][1]), glm::vec3(-1.0f, 0.0f, 0.0f), nxnynz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(textureIndices[blockID - 1][1]), glm::vec3(-1.0f, 0.0f, 0.0f), nxnypz),
                              });
            m_Offset += 4;
        }

        // +Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y + 1, block.z)) == m_Blocks.end())
        if (GetBlock({x, y + 1, z}))
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});

            glm::vec2 test = TextureMap::GetTopRight(blockID);

            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopLeft(textureIndices[blockID - 1][2]), glm::vec3(0.0f, +1.0f, 0.0f), pxpypz),
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopRight(textureIndices[blockID - 1][2]), glm::vec3(0.0f, +1.0f, 0.0f), pxpynz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetBottomRight(textureIndices[blockID - 1][2]), glm::vec3(0.0f, +1.0f, 0.0f), nxpynz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetBottomLeft(textureIndices[blockID - 1][2]), glm::vec3(0.0f, +1.0f, 0.0f), nxpypz),
                              });
            m_Offset += 4;
        }

        // -Y Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y - 1, block.z)) == m_Blocks.end())
        if (GetBlock({x, y - 1, z}))
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetTopRight(textureIndices[blockID - 1][3]), glm::vec3(0.0f, -1.0f, 0.0f), pxnypz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(textureIndices[blockID - 1][3]), glm::vec3(0.0f, -1.0f, 0.0f), nxnypz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(textureIndices[blockID - 1][3]), glm::vec3(0.0f, -1.0f, 0.0f), nxnynz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetTopLeft(textureIndices[blockID - 1][3]), glm::vec3(0.0f, -1.0f, 0.0f), pxnynz),
                              });
            m_Offset += 4;
        }

        // +Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z + 1)) == m_Blocks.end())
        if (GetBlock({x, y, z + 1}))
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopRight(textureIndices[blockID - 1][4]), glm::vec3(0.0f, 0.0f, +1.0f), pxpypz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), TextureMap::GetTopLeft(textureIndices[blockID - 1][4]), glm::vec3(0.0f, 0.0f, +1.0f), nxpypz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomLeft(textureIndices[blockID - 1][4]), glm::vec3(0.0f, 0.0f, +1.0f), nxnypz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), TextureMap::GetBottomRight(textureIndices[blockID - 1][4]), glm::vec3(0.0f, 0.0f, +1.0f), pxnypz),
                              });
            m_Offset += 4;
        }

        // -Z Quad
        if (m_Blocks.find(glm::ivec3(block.x, block.y, block.z - 1)) == m_Blocks.end())
        if (GetBlock({x, y, z - 1}))
        {
            m_Indices.insert(m_Indices.end(), {0 + m_Offset, 1 + m_Offset, 2 + m_Offset, 2 + m_Offset, 3 + m_Offset, 0 + m_Offset});
            m_Vertices.insert(m_Vertices.end(),
                              {
                                  Vertex(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopRight(textureIndices[blockID - 1][5]), glm::vec3(0.0f, 0.0f, -1.0f), pxpynz),
                                  Vertex(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomRight(textureIndices[blockID - 1][5]), glm::vec3(0.0f, 0.0f, -1.0f), pxnynz),
                                  Vertex(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), TextureMap::GetBottomLeft(textureIndices[blockID - 1][5]), glm::vec3(0.0f, 0.0f, -1.0f), nxnynz),
                                  Vertex(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), TextureMap::GetTopLeft(textureIndices[blockID - 1][5]), glm::vec3(0.0f, 0.0f, -1.0f), nxpynz),
                              });
            m_Offset += 4;
        }
    }
}
