#include "Vertex.h"

Vertex::Vertex(glm::vec3 position, glm::vec2 textureCoordinate, glm::vec3 normal)
    : m_Position(position),
      m_TextureCoordinate(textureCoordinate),
      m_Normal(normal)
{
}