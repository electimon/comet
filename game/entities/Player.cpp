#include "Player.h"

#include <thread>

Player::Player(glm::vec3 position) : m_Position(position)
{
    Camera::SetPosition(position);
    EntityHandler::AddEntityToQueue(this);

    m_RenderDistance = 10;
}

Player::~Player() {}

void Player::Update()
{
    if (MouseHandler::GetLeftClick())
    {
        Player::BreakBlock();
    }
    else if (MouseHandler::GetRightClick())
    {
        Player::PlaceBlock();
    }

    if (glfwGetKey(WindowHandler::GetGLFWWindow(), GLFW_KEY_HOME))
    {
        Camera::SetPosition({0.0f, 100.0f, 0.0f});
    }

    m_Position = Camera::GetPosition();

    glm::ivec3 newChunkIndex = World::GetChunkIndex(m_Position);

    if (newChunkIndex != m_ChunkIndex)
    {
        // Update requested chunks if the chunk index changes
        std::cout << "Entered chunk: " << newChunkIndex.x << " " << newChunkIndex.z << "\n";

        m_ChunkIndex = newChunkIndex;
        World::ProcessRequestedChunks(m_RenderDistance, m_ChunkIndex);
    }
}