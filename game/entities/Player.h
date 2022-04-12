#pragma once

#include <iostream>
#include <unordered_set>
#include <cmath>

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

    void BreakBlock()
    {
        float step = 1.0f / 16.0f;
        glm::vec3 direction = Camera::GetDirection();
        glm::vec3 position = Camera::GetPosition();

        while (glm::length(direction) < 5.0f)
        {
            direction += glm::normalize(direction) * step;
            if (World::GetBlock(round(position + direction)) != 0)
            {
                World::SetBlock(round(position + direction), 0);
                return;
            }
        }
    }

    void PlaceBlock()
    {
        float step{1.0f / 16.0f};
        glm::vec3 direction = Camera::GetDirection();
        glm::vec3 position = Camera::GetPosition();
        glm::vec3 positionLast = position;

        while (glm::length(direction) < 5.0f)
        {
            direction += glm::normalize(direction) * step;
            if (World::GetBlock(round(position + direction)) != 0)
            {
                World::SetBlock(round(positionLast), 1);
                return;
            }
            positionLast = position + direction;
        }
    }

private:
    glm::vec3 m_Position;
    glm::ivec3 m_ChunkIndex;
    std::unordered_set<glm::ivec3> m_RequestedChunks;
    int m_RenderDistance;
};