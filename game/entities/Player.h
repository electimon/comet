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

    void BreakBlock()
    {
        float step = 1.0f / 16.0f;
        glm::vec3 direction = Camera::Direction();
        glm::vec3 position = Camera::Position();

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
        glm::vec3 direction = Camera::Direction();
        glm::vec3 position = Camera::Position();
        glm::vec3 positionLast = position;

        bool first = false;

        while (glm::length(direction) < 5.0f)
        {
            direction += glm::normalize(direction) * step;
            if (World::GetBlock(round(position + direction)) != 0)
            {
                if (first)
                {
                    return;
                }

                World::SetBlock(round(positionLast), 1);
                return;
            }
            positionLast = position + direction;
            first = false;
        }
    }

    void GetRequestedChunks();

    int RenderDistance() const { return m_RenderDistance; }
    void SetRenderDistance(int RenderDistance) { m_RenderDistance = RenderDistance; }

private:
    // glm::vec3 m_Position;
    glm::ivec3 m_ChunkIndex;
    int m_RenderDistance;
};