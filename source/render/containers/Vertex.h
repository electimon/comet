#pragma once

#include "glm/glm.hpp"

struct Vertex
{
    Vertex();

    // Position
    Vertex(float xPos, float yPos, float zPos);

    // Position, Color
    Vertex(float xPos, float yPos, float zPos, float r, float g, float b);
    Vertex(float xPos, float yPos, float zPos, float r, float g, float b, float a);
    Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb);
    Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba);

    // Position, Color, Normal
    Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb, const glm::vec3 &normal);
    Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb, float xNormal, float yNormal, float zNormal);
    Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba, const glm::vec3 &normal);
    Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba, float xNormal, float yNormal, float zNormal);

    // Position, Color, Normal, AO
    Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb, const glm::vec3 &normal, float ao);
    Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb, float xNormal, float yNormal, float zNormal, float ao);
    Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba, const glm::vec3 &normal, float ao);
    Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba, float xNormal, float yNormal, float zNormal, float ao);

    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
    float ao;
};