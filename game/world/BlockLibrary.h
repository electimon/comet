#pragma once

#include <vector>
#include <unordered_map>

class BlockLibrary
{
public:
    static BlockLibrary &GetInstance()
    {
        static BlockLibrary s_Instance;
        return s_Instance;
    }

    static void Initialize()
    {
        std::vector<int> Debug{47, 47, 47, 47, 47, 47}; // 0
        GetInstance().m_BlockIndices.push_back(Debug);

        std::vector<int> Stone{1, 1, 1, 1, 1, 1}; // 1
        GetInstance().m_BlockIndices.push_back(Stone);

        std::vector<int> Grass{3, 3, 0, 2, 3, 3}; // 2
        GetInstance().m_BlockIndices.push_back(Grass);

        std::vector<int> Dirt{2, 2, 2, 2, 2, 2}; // 3
        GetInstance().m_BlockIndices.push_back(Dirt);

        std::vector<int> Water{207, 207, 207, 207, 207, 207}; // 4
        GetInstance().m_BlockIndices.push_back(Water);

        std::vector<int> Log{20, 20, 21, 21, 20, 20}; // 5
        GetInstance().m_BlockIndices.push_back(Log);

        std::vector<int> Leaves{53, 53, 53, 53, 53, 53}; // 6
        GetInstance().m_BlockIndices.push_back(Leaves);

        std::vector<int> Sand{18, 18, 18, 18, 18, 18}; // 7
        GetInstance().m_BlockIndices.push_back(Sand);

        std::vector<int> Bedrock{17, 17, 17, 17, 17, 17}; // 8
        GetInstance().m_BlockIndices.push_back(Bedrock);

        std::vector<int> Cobblestone{16, 16, 16, 16, 16, 16}; // 9
        GetInstance().m_BlockIndices.push_back(Cobblestone);

        std::vector<int> CoalOre{34, 34, 34, 34, 34, 34}; // 10
        GetInstance().m_BlockIndices.push_back(CoalOre);

        std::vector<int> IronOre{33, 33, 33, 33, 33, 33}; // 11
        GetInstance().m_BlockIndices.push_back(IronOre);

        std::vector<int> GoldOre{32, 32, 32, 32, 32, 32}; // 12
        GetInstance().m_BlockIndices.push_back(GoldOre);

        std::vector<int> DiamondOre{66, 66, 66, 66, 66, 66}; // 13
        GetInstance().m_BlockIndices.push_back(DiamondOre);
    }

    static std::vector<int> GetIndices(unsigned int blockID)
    {
        return GetInstance().m_BlockIndices[blockID];
    }

    void ProcessTextures();

    std::vector<int> GetTextureIDs(int blockID) { return m_BlockTextures[blockID]; }

    int GetTextureTop(int blockID) { return m_BlockTextures[blockID][0]; }
    int GetTextureBottom(int blockID) { return m_BlockTextures[blockID][1]; }
    int GetTextureLeft(int blockID) { return m_BlockTextures[blockID][2]; }
    int GetTextureRight(int blockID) { return m_BlockTextures[blockID][3]; }
    int GetTextureFront(int blockID) { return m_BlockTextures[blockID][4]; }
    int GetTextureBack(int blockID) { return m_BlockTextures[blockID][5]; }

private:
    BlockLibrary() {}
    BlockLibrary(BlockLibrary const &);
    void operator=(BlockLibrary const &);

    std::vector<std::vector<int>> m_BlockIndices;
    std::unordered_map<int, std::vector<int>> m_BlockTextures;
};