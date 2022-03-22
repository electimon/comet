#pragma once

#include <iostream>
#include <unordered_set>

#include "glm/glm.hpp"

#include "handlers/Entity.h"
#include "handlers/EntityHandler.h"

#include "world/Chunk.h"
#include "world/World.h"

class Player : public Entity
{
public:
    Player();
    Player(glm::vec3 position);
    ~Player();

    void Update();

    glm::vec3 GetPosition() { return m_Position; }
    void UpdateRequestedChunks();

    void SetWorld(World *world) { p_World = world; }

private:
    glm::vec3 m_Position;
    glm::ivec3 m_ChunkIndex;
    std::unordered_set<glm::ivec3> m_RequestedChunks;
    int m_RenderDistance;
    int m_ChunkSize;

    World *p_World;
};