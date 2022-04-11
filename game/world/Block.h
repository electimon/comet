#pragma once

class Block
{
public:
    Block(unsigned char id);
    ~Block();

    unsigned char GetID();

private:
    unsigned char m_ID;
};