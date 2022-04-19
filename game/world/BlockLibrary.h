#pragma once

#include <comet.pch>

class BlockLibrary
{
  public:
    inline static auto &Instance()
    {
        static BlockLibrary instance;
        return instance;
    }

    static void Initialize()
    {
        // Assigning block textures
        std::array<unsigned char, 6> Debug{47, 47, 47, 47, 47, 47};
        std::array<unsigned char, 6> Stone{1, 1, 1, 1, 1, 1};
        std::array<unsigned char, 6> Grass{3, 3, 0, 2, 3, 3};
        std::array<unsigned char, 6> Dirt{2, 2, 2, 2, 2, 2};
        std::array<unsigned char, 6> Water{207, 207, 207, 207, 207, 207};
        std::array<unsigned char, 6> Log{20, 20, 21, 21, 20, 20};
        std::array<unsigned char, 6> Leaves{53, 53, 53, 53, 53, 53};
        std::array<unsigned char, 6> Sand{18, 18, 18, 18, 18, 18};
        std::array<unsigned char, 6> Bedrock{17, 17, 17, 17, 17, 17};
        std::array<unsigned char, 6> Cobblestone{16, 16, 16, 16, 16, 16};
        std::array<unsigned char, 6> CoalOre{34, 34, 34, 34, 34, 34};
        std::array<unsigned char, 6> IronOre{33, 33, 33, 33, 33, 33};
        std::array<unsigned char, 6> GoldOre{32, 32, 32, 32, 32, 32};
        std::array<unsigned char, 6> DiamondOre{66, 66, 66, 66, 66, 66};

        // Assigning block IDs
        Instance().m_BlockIndices.at(0) = Debug;
        Instance().m_BlockIndices.at(1) = Stone;
        Instance().m_BlockIndices.at(2) = Grass;
        Instance().m_BlockIndices.at(3) = Dirt;
        Instance().m_BlockIndices.at(4) = Water;
        Instance().m_BlockIndices.at(5) = Log;
        Instance().m_BlockIndices.at(6) = Leaves;
        Instance().m_BlockIndices.at(7) = Sand;
        Instance().m_BlockIndices.at(8) = Bedrock;
        Instance().m_BlockIndices.at(9) = Cobblestone;
        Instance().m_BlockIndices.at(10) = CoalOre;
        Instance().m_BlockIndices.at(11) = IronOre;
        Instance().m_BlockIndices.at(12) = GoldOre;
        Instance().m_BlockIndices.at(13) = DiamondOre;
    }

    static std::array<unsigned char, 6> GetIndices(unsigned char blockID)
    {
        return Instance().m_BlockIndices.at(blockID);
    }

  private:
    BlockLibrary()
    {
    }
    BlockLibrary(BlockLibrary const &);
    void operator=(BlockLibrary const &);

    std::array<std::array<unsigned char, 6>, 256> m_BlockIndices;
};