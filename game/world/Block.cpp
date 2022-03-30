#include "Block.h"

Block::Block(unsigned __int8 id)
    : m_ID(id)
{
}

Block::~Block()
{
}

unsigned __int8 Block::GetID()
{
    return m_ID;
}