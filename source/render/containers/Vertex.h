#pragma once

#include "glm/glm.hpp"

struct Vertex
{
    Vertex();
    Vertex(float xPos, float yPos, float zPos);
    Vertex(float xPos, float yPos, float zPos, float r, float g, float b);

    glm::vec3 position;
    glm::vec3 color;
};