#include "Player.h"

#include <thread>

Player::Player()
{
    EntityHandler::AddEntityToQueue(this);

    m_RenderDistance = 2;
    m_ChunkSize = World::GetChunkSize();
}

Player::Player(glm::vec3 position)
    : m_Position(position)
{
    Camera::SetPosition(position);
    EntityHandler::AddEntityToQueue(this);

    m_RenderDistance = 2;
    m_ChunkSize = World::GetChunkSize();
}

Player::~Player()
{
}

void Player::Update()
{
    m_Position = Camera::GetPosition();

    glm::ivec3 newChunkIndex;

    if (m_Position.x < 0.0f)
    {
        newChunkIndex.x = ((float)m_Position.x - m_ChunkSize) / m_ChunkSize;
    }
    else
    {
        newChunkIndex.x = ((float)m_Position.x) / m_ChunkSize;
    }

    if (m_Position.z < 0.0f)
    {
        newChunkIndex.z = ((float)m_Position.z - m_ChunkSize) / m_ChunkSize;
    }
    else
    {
        newChunkIndex.z = ((float)m_Position.z) / m_ChunkSize;
    }

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

    // Pass set of new chunks to the world to deal with
    p_World->ProcessRequestedChunks(m_RequestedChunks);
}
