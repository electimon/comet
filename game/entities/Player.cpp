#include "Player.h"

#include <thread>

Player::Player(glm::vec3 position)
    : m_Position(position)
{
    Camera::SetPosition(position);
    EntityHandler::AddEntityToQueue(this);

    m_RenderDistance = 4;
}

Player::~Player()
{
}

void Player::Update()
{
    if (glfwGetMouseButton(WindowHandler::GetGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT))
    {
        if (m_Cooldown + 0.2 < glfwGetTime())
            Player::BreakBlock();
    }
    if (glfwGetKey(WindowHandler::GetGLFWWindow(), GLFW_KEY_HOME))
    {
        Camera::SetPosition({0.0f, 100.0f, 0.0f});
    }

    m_Position = Camera::GetPosition();

    glm::ivec3 newChunkIndex = World::GetChunkIndexFromWorldCoord(m_Position);

    if (newChunkIndex != m_ChunkIndex)
    {
        // Update requested chunks if the chunk index changes
        std::cout << "Entered chunk: " << newChunkIndex.x << " " << newChunkIndex.z << std::endl;

        m_ChunkIndex = newChunkIndex;
        UpdateRequestedChunks();
    }
}

void Player::UpdateRequestedChunks()
{
    // Begin new set of chunks to request
    m_RequestedChunks.clear();

    // Loop through square shape around the player
    for (int x = m_ChunkIndex.x - m_RenderDistance; x < 1 + m_ChunkIndex.x + m_RenderDistance; x++)
    {
        for (int z = m_ChunkIndex.z - m_RenderDistance; z < 1 + m_ChunkIndex.z + m_RenderDistance; z++)
        {
            m_RequestedChunks.insert(glm::ivec3(x, 0, z));
        }
    }

    p_World->ProcessRequestedChunks(m_RequestedChunks);
}
