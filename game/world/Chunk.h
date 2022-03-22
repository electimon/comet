#pragma once

#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include "containers/Mesh.h"

#include "Block.h"

struct Chunk
{
    Chunk(glm::ivec3 id);
    ~Chunk();

    void SetBlock(glm::ivec3 coordinate, Block &block);
    void SetBlock(int x, int y, int z, Block &block);
    void Redraw();

    // Feature functions
    void GenerateSurface();
    void GenerateSurfaceMesh();

    void GenerateWater();
    void GenerateTrees();

    void PlaceTree(int x, int y, int z);

    Mesh *GetMesh() { return &m_SolidMesh; }
    Mesh *GetTransparentMesh() { return &m_TransparentMesh; }

private:
    std::unordered_map<glm::ivec3, Block> m_Blocks;
    std::unordered_map<glm::ivec2, int> m_SurfaceHeights;

    glm::ivec3 m_Chunk;

    int m_ChunkSize;
    int m_ChunkHeight;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    unsigned int m_Offset;

    Mesh m_SolidMesh;
    Mesh m_TransparentMesh;
};