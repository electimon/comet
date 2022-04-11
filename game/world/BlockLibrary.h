#pragma once

#include <string>
#include <vector>

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
        std::vector<unsigned char> Debug{47, 47, 47, 47, 47, 47}; // 0
        GetInstance().m_BlockIndices.push_back(Debug);

        std::vector<unsigned char> Stone{1, 1, 1, 1, 1, 1}; // 1
        GetInstance().m_BlockIndices.push_back(Stone);

        std::vector<unsigned char> Grass{3, 3, 0, 2, 3, 3}; // 2
        GetInstance().m_BlockIndices.push_back(Grass);

        std::vector<unsigned char> Dirt{2, 2, 2, 2, 2, 2}; // 3
        GetInstance().m_BlockIndices.push_back(Dirt);

        std::vector<unsigned char> Water{207, 207, 207, 207, 207, 207}; // 4
        GetInstance().m_BlockIndices.push_back(Water);

        std::vector<unsigned char> Log{20, 20, 21, 21, 20, 20}; // 5
        GetInstance().m_BlockIndices.push_back(Log);

        std::vector<unsigned char> Leaves{53, 53, 53, 53, 53, 53}; // 6
        GetInstance().m_BlockIndices.push_back(Leaves);

        std::vector<unsigned char> Sand{18, 18, 18, 18, 18, 18}; // 7
        GetInstance().m_BlockIndices.push_back(Sand);

        std::vector<unsigned char> Bedrock{17, 17, 17, 17, 17, 17}; // 8
        GetInstance().m_BlockIndices.push_back(Bedrock);

        std::vector<unsigned char> Cobblestone{16, 16, 16, 16, 16, 16}; // 9
        GetInstance().m_BlockIndices.push_back(Cobblestone);

        std::vector<unsigned char> CoalOre{34, 34, 34, 34, 34, 34}; // 10
        GetInstance().m_BlockIndices.push_back(CoalOre);

        std::vector<unsigned char> IronOre{33, 33, 33, 33, 33, 33}; // 11
        GetInstance().m_BlockIndices.push_back(IronOre);

        std::vector<unsigned char> GoldOre{32, 32, 32, 32, 32, 32}; // 12
        GetInstance().m_BlockIndices.push_back(GoldOre);

        std::vector<unsigned char> DiamondOre{66, 66, 66, 66, 66, 66}; // 13
        GetInstance().m_BlockIndices.push_back(DiamondOre);
    }

    static std::vector<unsigned char> GetIndices(unsigned char blockID)
    {
        return GetInstance().m_BlockIndices[blockID];
    }

private:
    BlockLibrary() {}
    BlockLibrary(BlockLibrary const &);
    void operator=(BlockLibrary const &);

    std::vector<std::vector<unsigned char>> m_BlockIndices;
};