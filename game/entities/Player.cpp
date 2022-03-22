#include "Player.h"

Player::Player()
{
    EntityHandler::AddEntityToQueue(this);
}

Player::~Player()
{
}

void Player::Update()
{
    std::cout << "Player updated." << std::endl;
    m_Position = Camera::GetPosition();
}