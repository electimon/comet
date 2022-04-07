#pragma once

class Block {
public:
  Block(unsigned int id);
  ~Block();

  unsigned int GetID();

private:
  unsigned int m_ID;
};