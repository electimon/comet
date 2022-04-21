#pragma once

#include <comet.pch>

struct Vertex
{
    // Position, Texture Coordinates, Normal
    Vertex(glm::vec3 position, glm::vec2 textureCoordinate, glm::vec3 normal);

    glm::vec3 m_Position;
    glm::vec2 m_TextureCoordinate;
    glm::vec3 m_Normal;
};
