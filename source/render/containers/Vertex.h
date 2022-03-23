#pragma once

#include "glm/glm.hpp"

struct Vertex
{
    Vertex();
    Vertex(float xPos, float yPos, float zPos);
    Vertex(float xPos, float yPos, float zPos, float r, float g, float b);
    Vertex(float xPos, float yPos, float zPos, float r, float g, float b, float a);
    Vertex(float xPos, float yPos, float zPos, const glm::vec3 &color);
    Vertex(float xPos, float yPos, float zPos, const glm::vec4 &color);

    glm::vec3 position;
    glm::vec4 color;
};