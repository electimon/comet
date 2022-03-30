#pragma once

#include <vector>
#include <string>

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
        std::vector<unsigned __int8> Debug{47, 47, 47, 47, 47, 47}; // 0
        GetInstance().m_BlockIndices.push_back(Debug);

        std::vector<unsigned __int8> Stone{1, 1, 1, 1, 1, 1}; // 1
        GetInstance().m_BlockIndices.push_back(Stone);

        std::vector<unsigned __int8> Grass{3, 3, 0, 2, 3, 3}; // 2
        GetInstance().m_BlockIndices.push_back(Grass);

        std::vector<unsigned __int8> Dirt{2, 2, 2, 2, 2, 2}; // 3
        GetInstance().m_BlockIndices.push_back(Dirt);

        std::vector<unsigned __int8> Water{207, 207, 207, 207, 207, 207}; // 4
        GetInstance().m_BlockIndices.push_back(Water);

        std::vector<unsigned __int8> Log{20, 20, 21, 21, 20, 20}; // 5
        GetInstance().m_BlockIndices.push_back(Log);

        std::vector<unsigned __int8> Leaves{53, 53, 53, 53, 53, 53}; // 6
        GetInstance().m_BlockIndices.push_back(Leaves);

        std::vector<unsigned __int8> Sand{18, 18, 18, 18, 18, 18}; // 7
        GetInstance().m_BlockIndices.push_back(Sand);

        std::vector<unsigned __int8> Bedrock{17, 17, 17, 17, 17, 17}; // 8
        GetInstance().m_BlockIndices.push_back(Bedrock);

        std::vector<unsigned __int8> Cobblestone{16, 16, 16, 16, 16, 16}; // 9
        GetInstance().m_BlockIndices.push_back(Cobblestone);

        std::vector<unsigned __int8> CoalOre{34, 34, 34, 34, 34, 34}; // 10
        GetInstance().m_BlockIndices.push_back(CoalOre);

        std::vector<unsigned __int8> IronOre{33, 33, 33, 33, 33, 33}; // 11
        GetInstance().m_BlockIndices.push_back(IronOre);

        std::vector<unsigned __int8> GoldOre{32, 32, 32, 32, 32, 32}; // 12
        GetInstance().m_BlockIndices.push_back(GoldOre);

        std::vector<unsigned __int8> DiamondOre{66, 66, 66, 66, 66, 66}; // 13
        GetInstance().m_BlockIndices.push_back(DiamondOre);
    }

    static std::vector<unsigned __int8> GetIndices(unsigned __int8 blockID)
    {
        return GetInstance().m_BlockIndices[blockID];
    }

private:
    BlockLibrary() {}
    BlockLibrary(BlockLibrary const &);
    void operator=(BlockLibrary const &);

    std::vector<std::vector<unsigned __int8>> m_BlockIndices;
};