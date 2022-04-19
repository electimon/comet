#include "Block.h"

Block::Block() {}

Block::Block(unsigned char id) : m_ID(id) { m_Transparent = (id == 0 || id == 4 || id == 6) ? true : false; }

Block::Block(unsigned char id, bool transparent) : m_ID(id), m_Transparent(transparent) {}

Block::~Block() {}