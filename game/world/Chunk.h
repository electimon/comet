#pragma once

#include <string>

#include "Renderer.h"
#include "render/TextureMap.h"
#include "render/Vertex.h"
#include "Timer.h"

#include "Block.h"
#include "BlockLibrary.h"
#include "ChunkGenerator.h"
#include "WorldConfig.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/array.hpp>

struct Geometry
{
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    unsigned int Offset;
};

struct Chunk
{
    Chunk(glm::ivec3 id);
    ~Chunk();

    // For use by world manager
    void Allocate();
    void Generate();

    void GenerateSurface();
    void GenerateTrees();
    void GenerateBedrock();
    void GenerateCaves();
    void GenerateWater();
    void GenerateSand();

    void GenerateMesh();

    Block GetBlock(glm::ivec3 chunkPos)
    {
        if (chunkPos.x < 0 || chunkPos.y < 0 || chunkPos.z < 0)
        {
            return Block(0, true);
        }
        if (chunkPos.x == CHUNK_WIDTH || chunkPos.y == CHUNK_HEIGHT || chunkPos.z == CHUNK_WIDTH)
        {
            return Block(0, true);
        }

        return m_BlockData.at(chunkPos.x * CHUNK_HEIGHT * CHUNK_WIDTH + chunkPos.y * CHUNK_WIDTH + chunkPos.z);
    }

    inline void SetBlock(glm::ivec3 chunkPos, Block block)
    {
        m_BlockData.at(chunkPos.x * CHUNK_HEIGHT * CHUNK_WIDTH + chunkPos.y * CHUNK_WIDTH + chunkPos.z) = block;
    }

    inline void SetHeight(int x, int z, int y) { m_HeightData.at(CHUNK_WIDTH * x + z) = y; }
    float GetHeight(int x, int z) { return m_HeightData.at(CHUNK_WIDTH * x + z); }

  private:
    std::array<Block, CHUNK_SIZE> m_BlockData;

    // only used during generation, not needed when saving chunk
    std::array<int, CHUNK_WIDTH * CHUNK_WIDTH> m_HeightData;

    Geometry m_SolidGeometry;
    Geometry m_TransparentGeometry;

    // Flag to check if the chunk needs to be saved to disk or not
    bool m_Modified = false;
    bool m_Generated = false;

    glm::ivec3 m_Chunk = {0, 0, 0};

  public:
    bool IsModified() const { return m_Modified; }
    void SetModified(bool Modified) { m_Modified = Modified; }

    bool IsGenerated() const { return m_Generated; }
    void SetGenerated(bool Generated) { m_Generated = Generated; }

    Geometry *SolidGeometry() { return &m_SolidGeometry; }
    Geometry *TransparentGeometry() { return &m_TransparentGeometry; }

    template <class Archive> void save(Archive &ar) const { ar(m_BlockData); }
    template <class Archive> void load(Archive &ar) { ar(m_BlockData); }
};
