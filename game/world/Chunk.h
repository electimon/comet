#pragma once

#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "glm/gtc/random.hpp"

#include "Engine.h"

#include "render/Renderer.h"
#include "render/containers/Vertex.h"
#include "render/containers/Mesh.h"

#include "Block.h"

struct Chunk
{
    Chunk(glm::ivec3 id);
    ~Chunk();

    void SetBlock(glm::ivec3 coordinate, Block &block);
    void FillChunk();
    void GenerateMesh();
    void Redraw();

    Mesh *GetMesh() { return &m_Mesh; }

private:
    std::unordered_map<glm::ivec3, Block> m_Blocks;
    glm::ivec3 m_Chunk;
    int m_ChunkSize;
    int m_ChunkHeight;

    Mesh m_Mesh;
};