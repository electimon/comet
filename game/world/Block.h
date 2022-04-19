#pragma once

class Block
{
  public:
    Block();
    Block(unsigned char id);
    Block(unsigned char id, bool transparent);
    ~Block();

    unsigned char ID() const { return m_ID; }
    void SetID(unsigned char ID) { m_ID = ID; }

    bool IsTransparent() const { return m_Transparent; }
    void SetTransparent(bool Transparent) { m_Transparent = Transparent; }

  private:
    unsigned char m_ID;
    bool m_Transparent;
};