#pragma once

#include "glm/glm.hpp"

struct Vertex
{
    Vertex()
    {
    }
    Vertex(float xPos, float yPos, float zPos, float r, float g, float b)
        : position(xPos, yPos, zPos),
          color(r, g, b)
    {
    }
    Vertex(float xPos, float yPos, float zPos)
        : position(xPos, yPos, zPos),
          color(1.0f, 1.0f, 1.0f)
    {
    }
    glm::vec3 position;
    glm::vec3 color;
};