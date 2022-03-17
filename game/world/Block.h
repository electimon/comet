#pragma once

#include <iostream>

class Block
{
public:
    Block(unsigned int id)
        : m_ID(id)
    {
        // std::cout << "Block::Block()" << std::endl;
    }

    ~Block()
    {
        // std::cout << "Block::~Block()" << std::endl;
    }

private:
    unsigned int m_ID;
};