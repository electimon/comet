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
        std::vector<int> Debug{16, 16, 16, 16, 16, 16}; // 0
        GetInstance().m_BlockIndices.push_back(Debug);

        std::vector<int> Stone{241, 241, 241, 241, 241, 241}; // 1
        GetInstance().m_BlockIndices.push_back(Stone);

        std::vector<int> Grass{243, 243, 240, 242, 243, 243}; // 2
        GetInstance().m_BlockIndices.push_back(Grass);

        std::vector<int> Dirt{242, 242, 242, 242, 242, 242}; // 3
        GetInstance().m_BlockIndices.push_back(Dirt);

        std::vector<int> Water{63, 63, 63, 63, 63, 63}; // 4
        GetInstance().m_BlockIndices.push_back(Water);

        std::vector<int> Log{228, 228, 229, 229, 228, 228}; // 5
        GetInstance().m_BlockIndices.push_back(Log);

        std::vector<int> Leaves{197, 197, 197, 197, 197, 197}; // 6
        GetInstance().m_BlockIndices.push_back(Leaves);

        std::vector<int> Sand{226, 226, 226, 226, 226, 226}; // 7
        GetInstance().m_BlockIndices.push_back(Sand);

        std::vector<int> Bedrock{225, 225, 225, 225, 225, 225}; // 8
        GetInstance().m_BlockIndices.push_back(Bedrock);

        std::vector<int> Cobblestone{224, 224, 224, 224, 224, 224}; // 9
        GetInstance().m_BlockIndices.push_back(Cobblestone);

        std::vector<int> CoalOre{210, 210, 210, 210, 210, 210}; // 10
        GetInstance().m_BlockIndices.push_back(CoalOre);

        std::vector<int> IronOre{209, 209, 209, 209, 209, 209}; // 11
        GetInstance().m_BlockIndices.push_back(IronOre);

        std::vector<int> GoldOre{208, 208, 208, 208, 208, 208}; // 12
        GetInstance().m_BlockIndices.push_back(GoldOre);

        std::vector<int> DiamondOre{194, 194, 194, 194, 194, 194}; // 13
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