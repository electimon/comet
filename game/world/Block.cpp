#include "Block.h"

Block::Block(unsigned char id)
    : m_ID(id)
{
}

Block::~Block()
{
}

unsigned char Block::GetID()
{
    return m_ID;
}