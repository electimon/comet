#pragma once

#include <iostream>

class Block
{
public:
    Block(unsigned int id);
    ~Block();
    
    int GetID();

private:
    unsigned int m_ID;
};