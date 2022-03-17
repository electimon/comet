#pragma once

#include "glm/glm.hpp"

struct Vertex
{
    Vertex()
    {
    }
    Vertex(float xPos, float yPos, float zPos)
        : position(xPos, yPos, zPos)
    {
    }
    glm::vec3 position;
    // glm::vec3 color;
};