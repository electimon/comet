#include "Player.h"

Player::Player()
{
    Camera::SetPosition(m_Position);
    EntityHandler::AddEntityToQueue(this);
}

Player::~Player()
{
}

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

    m_Position = Camera::Position();

    GetRequestedChunks();
}

void Player::GetRequestedChunks()
{
    glm::ivec3 newChunkIndex = World::GetChunkIndex(m_Position);

    if (newChunkIndex != m_ChunkIndex)
    {
        // Update requested chunks if the chunk index changes
        std::cout << "Entered chunk: " << newChunkIndex.x << " " << newChunkIndex.z << "\n";

        m_ChunkIndex = newChunkIndex;
        World::ProcessRequestedChunks(m_RenderDistance, m_ChunkIndex);
    }
}
