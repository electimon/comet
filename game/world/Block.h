#pragma once

class Block
{
  public:
    Block();
    Block(unsigned char id);
    Block(unsigned char id, bool transparent);
    Block(unsigned char id, bool transparent, bool solid);
    ~Block();

    template <class Archive> void serialize(Archive &ar) { ar(m_ID, m_Transparent); }

  private:
    unsigned char m_ID;
    bool m_Transparent;
    bool m_Solid;

  public:
    unsigned char ID() const { return m_ID; }
    void SetID(unsigned char ID) { m_ID = ID; }

    bool IsTransparent() const { return m_Transparent; }
    void SetTransparent(bool Transparent) { m_Transparent = Transparent; }

    bool IsSolid() const { return m_Solid; }
    void SetSolid(bool Solid) { m_Solid = Solid; }
};
