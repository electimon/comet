#include "Block.h"

Block::Block() {}

Block::Block(unsigned char id) : m_ID(id) {}

Block::Block(unsigned char id, bool transparent) : m_ID(id), m_Transparent(transparent) {}

Block::~Block() {}