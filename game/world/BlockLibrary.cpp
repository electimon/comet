#include "BlockLibrary.h"

BlockLibrary::BlockLibrary()
{
    ProcessTextures();
}

BlockLibrary::~BlockLibrary()
{
}

void BlockLibrary::ProcessTextures()
{
    // top bottom left right front back
    m_BlockTextures[0] = {16, 16, 16, 16, 16, 16};       // debug
    m_BlockTextures[1] = {241, 241, 241, 241, 241, 241}; // stone
    m_BlockTextures[3] = {240, 242, 241, 241, 241, 241}; // grass
    m_BlockTextures[2] = {242, 242, 242, 242, 242, 242}; // dirt
    m_BlockTextures[4] = {63, 63, 63, 63, 63, 63};       // water
    m_BlockTextures[5] = {249, 249, 248, 248, 248, 248}; // log
    m_BlockTextures[6] = {196, 196, 196, 196, 196, 196}; // leaves
    m_BlockTextures[7] = {246, 246, 246, 246, 246, 246}; // sand
}