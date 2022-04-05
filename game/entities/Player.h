#pragma once

#include <iostream>
#include <unordered_set>

#include "glm/glm.hpp"

#include "handlers/Entity.h"
#include "handlers/EntityHandler.h"
#include "handlers/MouseHandler.h"

#include "world/Chunk.h"
#include "world/World.h"

class Player : public Entity
{
public:
    Player(glm::vec3 position);
    ~Player();

    void Update();

    glm::vec3 GetPosition() { return m_Position; }
    void UpdateRequestedChunks();

    void SetWorld(World *world) { p_World = world; }

    void BreakBlock()
    {
        float step = 0.01f;
        glm::vec3 direction = Camera::GetDirection();
        glm::vec3 position = Camera::GetPosition();

        while (glm::length(direction) < 5.0f)
        {
            direction += direction * step;
            if (p_World->GetBlock(position + direction) != 0)
            {
                p_World->SetBlock(position + direction, 0);
                return;
            }
        }
    }

private:
    glm::vec3 m_Position;
    glm::ivec3 m_ChunkIndex;
    std::unordered_set<glm::ivec3> m_RequestedChunks;
    int m_RenderDistance;

    World *p_World;
};