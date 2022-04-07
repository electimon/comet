#include "Block.h"

Block::Block(unsigned int id) : m_ID(id) {}

Block::~Block() {}

unsigned int Block::GetID() { return m_ID; }