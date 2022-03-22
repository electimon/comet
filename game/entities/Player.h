#pragma once

#include <iostream>

#include "glm/glm.hpp"

#include "handlers/Entity.h"
#include "handlers/EntityHandler.h"

class Player : public Entity
{
public:
    Player();
    ~Player();

    void Update();

private:
    glm::vec3 m_Position;
    glm::ivec3 m_ChunkIndex;

};