#pragma once

#include <comet.pch>

// Enum for convenience
enum ID
{
    Air,         // 0
    Stone,       // 1
    Grass,       // 2
    Dirt,        // 3
    Cobblestone, // 4
    Oak_Planks,  // 5
    Oak_Sapling, // 6
    // Spruce_Sapling, //
    // Birch_Sapling,  //
    Bedrock,  // 7
    Water,    // 8
    Water2,   // 9
    Lava,     // 10
    Lava2,    // 11
    Sand,     // 12
    Gravel,   // 13
    Gold_Ore, // 14
    Iron_Ore, // 15
    Coal_Ore, // 16
    Oak_Log,  // 17
    // Spruce_Log,
    // Birch_Log,
    Oak_Leaves, // 18
    // Spruce_Leaves,
    // Birch_Leaves,
    Sponge,             // 19
    Glass,              // 20
    Lapis_Lazuli_Ore,   // 21
    Lapis_Lazuli_Block, // 22
    Dispenser,          // 23
    Sandstone,          // 24
    Noteblock,          // 25
    _Bed,               // 26
    Powered_Rail,       // 27
    Detector_Rail,      // 28
    Stickey_Piston,     // 29
    Cobweb,             // 30
    _Dead_Oak_Sapling,  // 31
    // _Grass,
    // _Fern,
    _Dead_Sapling, // 32
    Piston,        // 33
    _Piston_Head,  // 34
    White_Wool,    // 35
    // Orange_Wool,
    // Magenta_Wool,
    // Light_Blue_Wool,
    // Yellow_Wool,
    // Light_Green_Wool,
    // Pink_Wool,
    // Dark_Gray_Wool,
    // Light_Gray_Wool,
    // Cyan_Wool,
    // Purple_Wool,
    // Blue_Wool,
    // Brown_Wool,
    // Dark_Green_Wool,
    // Red_Wool,
    // Black_Wool,
    _Grass_Top,          // 36
    Yellow_Flower,       // 37
    Red_Flower,          // 38
    Brown_Mushroom,      // 39
    Red_Mushroom,        // 40
    Gold_Block,          // 41
    Iron_Block,          // 42
    Stone_Stacked_Slabs, // 43
    // Sandstone_Stacked_Slabs,
    // Oak_Stacked_Slabs,
    // Cobblestone_Stacked_Slabs,
    Smoothstone_Slab, // 44
    // Sandstone_Slab,
    // Oak_Plank_Slab,
    // Cobblestone_Slab,
    Bricks,            // 45
    TNT,               // 46
    Bookshelf,         // 47
    Mossy_Cobblestone, // 48
    Obsidian,          // 49
    Torch,             // 50
    Fire,              // 51
    Monster_Spawner,   // 52
    Oak_Stairs,        // 53
    Chest,             // 54
    _Redstone,         // 55
    Diamond_Ore,       // 56
    Diamond_Block,     // 57
    Crafting_Table,    // 58
};

class BlockTextures
{
  public:
    inline static auto &Instance()
    {
        static BlockTextures instance;
        return instance;
    }

    static void Initialize()
    {
        // Assigning block textures
        std::array<unsigned char, 6> Air{0, 0, 0, 0, 0, 0};
        std::array<unsigned char, 6> Stone{1, 1, 1, 1, 1, 1};
        std::array<unsigned char, 6> Grass{3, 3, 0, 2, 3, 3};
        std::array<unsigned char, 6> Dirt{2, 2, 2, 2, 2, 2};
        std::array<unsigned char, 6> Cobblestone{16, 16, 16, 16, 16, 16};
        std::array<unsigned char, 6> Oak_Planks{4, 4, 4, 4, 4, 4};
        std::array<unsigned char, 6> Oak_Sapling{15, 0, 0, 0, 0, 0};
        std::array<unsigned char, 6> Bedrock{17, 17, 17, 17, 17, 17};
        std::array<unsigned char, 6> Water{207, 207, 207, 207, 207, 207};
        std::array<unsigned char, 6> Water2{207, 207, 207, 207, 207, 207};
        std::array<unsigned char, 6> Lava{255, 255, 255, 255, 255, 255};
        std::array<unsigned char, 6> Lava2{255, 255, 255, 255, 255, 255};
        std::array<unsigned char, 6> Sand{18, 18, 18, 18, 18, 18};
        std::array<unsigned char, 6> Gravel{19, 19, 19, 19, 19, 19};
        std::array<unsigned char, 6> Gold_Ore{32, 32, 32, 32, 32, 32};
        std::array<unsigned char, 6> Iron_Ore{33, 33, 33, 33, 33, 33};
        std::array<unsigned char, 6> Coal_Ore{34, 34, 34, 34, 34, 34};
        std::array<unsigned char, 6> Oak_Log{20, 20, 21, 21, 20, 20};
        std::array<unsigned char, 6> Oak_Leaves{52, 52, 52, 52, 52, 52};
        std::array<unsigned char, 6> Sponge{48, 48, 48, 48, 48, 48};
        std::array<unsigned char, 6> Glass{49, 49, 49, 49, 49, 49};
        std::array<unsigned char, 6> Lapis_Lazuli_Ore{160, 160, 160, 160, 160, 160};
        std::array<unsigned char, 6> Lapis_Lazuli_Block{154, 154, 154, 154, 154, 154};
        std::array<unsigned char, 6> Dispenser{46, 45, 1, 1, 45, 45};
        std::array<unsigned char, 6> Sandstone{192, 192, 176, 208, 192, 192};
        std::array<unsigned char, 6> Noteblock{58, 58, 59, 58, 58, 58};
        std::array<unsigned char, 6> _Bed{58, 58, 59, 58, 58, 58};
        std::array<unsigned char, 6> Powered_Rail{0, 0, 163, 0, 0, 0};

        // Assigning block IDs
        Instance().m_BlockIndices.at(ID::Air) = std::array<unsigned char, 6>{0, 0, 0, 0, 0, 0};
        Instance().m_BlockIndices.at(ID::Stone) = std::array<unsigned char, 6>{1, 1, 1, 1, 1, 1};
        Instance().m_BlockIndices.at(ID::Grass) = std::array<unsigned char, 6>{3, 3, 0, 2, 3, 3};
        Instance().m_BlockIndices.at(ID::Dirt) = std::array<unsigned char, 6>{2, 2, 2, 2, 2, 2};
        Instance().m_BlockIndices.at(ID::Cobblestone) = std::array<unsigned char, 6>{16, 16, 16, 16, 16, 16};
        Instance().m_BlockIndices.at(ID::Oak_Planks) = std::array<unsigned char, 6>{4, 4, 4, 4, 4, 4};
        Instance().m_BlockIndices.at(ID::Oak_Sapling) = std::array<unsigned char, 6>{15, 0, 0, 0, 0, 0};
        Instance().m_BlockIndices.at(ID::Bedrock) = std::array<unsigned char, 6>{17, 17, 17, 17, 17, 17};
        Instance().m_BlockIndices.at(ID::Water) = std::array<unsigned char, 6>{207, 207, 207, 207, 207, 207};
        Instance().m_BlockIndices.at(ID::Water2) = std::array<unsigned char, 6>{207, 207, 207, 207, 207, 207};
        Instance().m_BlockIndices.at(ID::Lava) = std::array<unsigned char, 6>{255, 255, 255, 255, 255, 255};
        Instance().m_BlockIndices.at(ID::Lava2) = std::array<unsigned char, 6>{255, 255, 255, 255, 255, 255};
        Instance().m_BlockIndices.at(ID::Sand) = std::array<unsigned char, 6>{18, 18, 18, 18, 18, 18};
        Instance().m_BlockIndices.at(ID::Gravel) = std::array<unsigned char, 6>{19, 19, 19, 19, 19, 19};
        Instance().m_BlockIndices.at(ID::Gold_Ore) = std::array<unsigned char, 6>{32, 32, 32, 32, 32, 32};
        Instance().m_BlockIndices.at(ID::Iron_Ore) = std::array<unsigned char, 6>{33, 33, 33, 33, 33, 33};
        Instance().m_BlockIndices.at(ID::Coal_Ore) = std::array<unsigned char, 6>{34, 34, 34, 34, 34, 34};
        Instance().m_BlockIndices.at(ID::Oak_Log) = std::array<unsigned char, 6>{20, 20, 21, 21, 20, 20};
        Instance().m_BlockIndices.at(ID::Oak_Leaves) = std::array<unsigned char, 6>{52, 52, 52, 52, 52, 52};
        Instance().m_BlockIndices.at(ID::Sponge) = std::array<unsigned char, 6>{48, 48, 48, 48, 48, 48};
        Instance().m_BlockIndices.at(ID::Glass) = std::array<unsigned char, 6>{49, 49, 49, 49, 49, 49};
        Instance().m_BlockIndices.at(ID::Lapis_Lazuli_Ore) = std::array<unsigned char, 6>{160, 160, 160, 160, 160, 160};
        Instance().m_BlockIndices.at(ID::Lapis_Lazuli_Block) =
            std::array<unsigned char, 6>{154, 154, 154, 154, 154, 154};
        Instance().m_BlockIndices.at(ID::Dispenser) = std::array<unsigned char, 6>{46, 45, 1, 1, 45, 45};
        Instance().m_BlockIndices.at(ID::Sandstone) = std::array<unsigned char, 6>{192, 192, 176, 208, 192, 192};
        Instance().m_BlockIndices.at(ID::Noteblock) = std::array<unsigned char, 6>{58, 58, 59, 58, 58, 58};
        Instance().m_BlockIndices.at(ID::_Bed) = std::array<unsigned char, 6>{58, 58, 59, 58, 58, 58};
        Instance().m_BlockIndices.at(ID::Powered_Rail) = std::array<unsigned char, 6>{0, 0, 163, 0, 0, 0};
        // Instance().m_BlockIndices.at(18) = Detector_Rail;
    }

    static std::array<unsigned char, 6> GetIndices(unsigned char blockID)
    {
        return Instance().m_BlockIndices.at(blockID);
    }

  private:
    BlockTextures(){};
    BlockTextures(BlockTextures const &);
    void operator=(BlockTextures const &);

    std::array<std::array<unsigned char, 6>, 256> m_BlockIndices;
};