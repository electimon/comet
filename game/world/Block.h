#pragma once

class Block
{
public:
    Block(unsigned __int8 id);
    ~Block();

    unsigned __int8 GetID();

private:
    unsigned __int8 m_ID;
};