#pragma once

#include <comet.pch>

class Entity
{
  public:
    Entity() {}
    ~Entity() {}

    virtual void Update() {}
    virtual void FrameUpdate() {}

  protected:
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};

  public:
    glm::vec3 Position() const { return m_Position; }
    void SetPosition(const glm::vec3 &Position) { m_Position = Position; }
};
