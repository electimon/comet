#include "Block.h"

Block::Block(unsigned int id)
    : m_ID(id)
{
    // std::cout << "Block::Block()" << std::endl;
}

Block::~Block()
{
    // std::cout << "Block::~Block()" << std::endl;
}

int Block::GetID()
{
    return m_ID;
}