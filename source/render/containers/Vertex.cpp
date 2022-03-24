#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(float xPos, float yPos, float zPos)
    : position(xPos, yPos, zPos),
      color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, float r, float g, float b)
    : position(xPos, yPos, zPos),
      color(r, g, b, 1.0f)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, float r, float g, float b, float a)
    : position(xPos, yPos, zPos),
      color(r, g, b, a)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb)
    : position(xPos, yPos, zPos),
      color(rgb, 1.0f)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba)
    : position(xPos, yPos, zPos),
      color(rgba)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba, const glm::vec3 &normal, float ao)
    : position(xPos, yPos, zPos),
      normal(normal),
      color(rgba),
      ao(ao)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, const glm::vec4 &rgba, float xNormal, float yNormal, float zNormal, float ao)
    : position(xPos, yPos, zPos),
      color(rgba),
      normal(xNormal, yNormal, zNormal),
      ao(ao)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb, const glm::vec3 &normal, float ao)
    : position(xPos, yPos, zPos),
      color(rgb, 1.0f),
      normal(normal),
      ao(ao)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, const glm::vec3 &rgb, float xNormal, float yNormal, float zNormal, float ao)
    : position(xPos, yPos, zPos),
      color(rgb, 1.0f),
      normal(xNormal, yNormal, zNormal),
      ao(ao)
{
}