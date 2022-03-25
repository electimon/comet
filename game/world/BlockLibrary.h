#pragma once

#include <vector>
#include <unordered_map>

class BlockLibrary
{
public:
    BlockLibrary();
    ~BlockLibrary();

    void ProcessTextures();

    std::vector<int> GetTextureIDs(int blockID) { return m_BlockTextures[blockID]; }

    int GetTextureTop(int blockID) { return m_BlockTextures[blockID][0]; }
    int GetTextureBottom(int blockID) { return m_BlockTextures[blockID][1]; }
    int GetTextureLeft(int blockID) { return m_BlockTextures[blockID][2]; }
    int GetTextureRight(int blockID) { return m_BlockTextures[blockID][3]; }
    int GetTextureFront(int blockID) { return m_BlockTextures[blockID][4]; }
    int GetTextureBack(int blockID) { return m_BlockTextures[blockID][5]; }


private:
    std::unordered_map<int, std::vector<int>> m_BlockTextures;
};