#pragma once

#include <comet.pch>

#include "handlers/Entity.h"
#include "handlers/EntityHandler.h"
#include "handlers/MouseHandler.h"

#include "world/Chunk.h"
#include "world/World.h"

class Player : public Entity
{
  public:
    Player();
    ~Player();

    void Update() override;

    void PlaceBlock();
    void BreakBlock();
    void ProcessScrolling();

    void GetRequestedChunks();

  private:
    // Purposfully setting this to an invalid index so that an update happens when
    // spawning in chunk 0, 0, 0
    glm::ivec3 m_ChunkIndex = {0, 1, 0};
    int m_RenderDistance;
    Block m_SelectedBlock = 1;

    double oldOffset = 0.0;
    double newOffset = 0.0;

  public:
    int RenderDistance() const { return m_RenderDistance; }
    void SetRenderDistance(int RenderDistance) { m_RenderDistance = RenderDistance; }

    glm::ivec3 ChunkIndex() const { return m_ChunkIndex; }
    void SetChunkIndex(const glm::ivec3 &ChunkIndex) { m_ChunkIndex = ChunkIndex; }

    Block SelectedBlock() const { return m_SelectedBlock; }
    void SetSelectedBlock(const Block &SelectedBlock) { m_SelectedBlock = SelectedBlock; }


};