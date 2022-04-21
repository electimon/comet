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
    _Grass_Top,                // 36
    Yellow_Flower,             // 37
    Red_Flower,                // 38
    Brown_Mushroom,            // 39
    Red_Mushroom,              // 40
    Gold_Block,                // 41
    Iron_Block,                // 42
    Smoothstone_Stacked_Slabs, // 43
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
        // Assigning block IDs
        Instance().m_BlockIndices.at(ID::Air) = {0, 0, 0, 0, 0, 0};
        Instance().m_BlockIndices.at(ID::Stone) = {1, 1, 1, 1, 1, 1};
        Instance().m_BlockIndices.at(ID::Grass) = {3, 3, 0, 2, 3, 3};
        Instance().m_BlockIndices.at(ID::Dirt) = {2, 2, 2, 2, 2, 2};
        Instance().m_BlockIndices.at(ID::Cobblestone) = {16, 16, 16, 16, 16, 16};
        Instance().m_BlockIndices.at(ID::Oak_Planks) = {4, 4, 4, 4, 4, 4};
        Instance().m_BlockIndices.at(ID::Oak_Sapling) = {15, 15, 15, 15, 15, 15};
        Instance().m_BlockIndices.at(ID::Bedrock) = {17, 17, 17, 17, 17, 17};
        Instance().m_BlockIndices.at(ID::Water) = {207, 207, 207, 207, 207, 207};
        Instance().m_BlockIndices.at(ID::Water2) = {207, 207, 207, 207, 207, 207};
        Instance().m_BlockIndices.at(ID::Lava) = {255, 255, 255, 255, 255, 255};
        Instance().m_BlockIndices.at(ID::Lava2) = {255, 255, 255, 255, 255, 255};
        Instance().m_BlockIndices.at(ID::Sand) = {18, 18, 18, 18, 18, 18};
        Instance().m_BlockIndices.at(ID::Gravel) = {19, 19, 19, 19, 19, 19};
        Instance().m_BlockIndices.at(ID::Gold_Ore) = {32, 32, 32, 32, 32, 32};
        Instance().m_BlockIndices.at(ID::Iron_Ore) = {33, 33, 33, 33, 33, 33};
        Instance().m_BlockIndices.at(ID::Coal_Ore) = {34, 34, 34, 34, 34, 34};
        Instance().m_BlockIndices.at(ID::Oak_Log) = {20, 20, 21, 21, 20, 20};
        Instance().m_BlockIndices.at(ID::Oak_Leaves) = {52, 52, 52, 52, 52, 52};
        Instance().m_BlockIndices.at(ID::Sponge) = {48, 48, 48, 48, 48, 48};
        Instance().m_BlockIndices.at(ID::Glass) = {49, 49, 49, 49, 49, 49};
        Instance().m_BlockIndices.at(ID::Lapis_Lazuli_Ore) = {160, 160, 160, 160, 160, 160};
        Instance().m_BlockIndices.at(ID::Lapis_Lazuli_Block) = {144, 144, 144, 144, 144, 144};
        Instance().m_BlockIndices.at(ID::Dispenser) = {46, 45, 1, 1, 45, 45};
        Instance().m_BlockIndices.at(ID::Sandstone) = {192, 192, 176, 208, 192, 192};
        Instance().m_BlockIndices.at(ID::Noteblock) = {74, 74, 75, 74, 74, 74};
        Instance().m_BlockIndices.at(ID::_Bed) = {58, 58, 59, 58, 58, 58};
        Instance().m_BlockIndices.at(ID::Powered_Rail) = {163, 163, 163, 163, 163, 163};
        Instance().m_BlockIndices.at(ID::Detector_Rail) = {195, 195, 195, 195, 195, 195};
        Instance().m_BlockIndices.at(ID::Stickey_Piston) = {84, 84, 82, 85, 84, 84}; // Wrong
        Instance().m_BlockIndices.at(ID::Cobweb) = {11, 11, 11, 11, 11, 11};
        Instance().m_BlockIndices.at(ID::_Dead_Oak_Sapling) = {43, 43, 43, 43, 43, 43}; // Wrong
        Instance().m_BlockIndices.at(ID::_Dead_Sapling) = {43, 43, 43, 43, 43, 43}; // Wrong
        Instance().m_BlockIndices.at(ID::Piston) = {84, 84, 83, 85, 84, 84}; // Wrong
        Instance().m_BlockIndices.at(ID::_Piston_Head) = {83, 83, 83, 83, 83, 83}; // Wrong
        Instance().m_BlockIndices.at(ID::White_Wool) = {64, 64, 64, 64, 64, 64};
        Instance().m_BlockIndices.at(ID::_Grass_Top) = {0, 0, 0, 0, 0, 0};
        Instance().m_BlockIndices.at(ID::Yellow_Flower) = {13, 13, 13, 13, 13, 13};
        Instance().m_BlockIndices.at(ID::Red_Flower) = {12, 12, 12, 12, 12, 12};
        Instance().m_BlockIndices.at(ID::Brown_Mushroom) = {29, 29, 29, 29, 29, 29};
        Instance().m_BlockIndices.at(ID::Red_Mushroom) = {28, 28, 28, 28, 28, 28};
        Instance().m_BlockIndices.at(ID::Gold_Block) = {23, 23, 23, 23, 23, 23};
        Instance().m_BlockIndices.at(ID::Iron_Block) = {22, 22, 22, 22, 22, 22};
        Instance().m_BlockIndices.at(ID::Smoothstone_Stacked_Slabs) = {5, 5, 6, 6, 5, 5};
        Instance().m_BlockIndices.at(ID::Smoothstone_Slab) = {5, 5, 6, 6, 5, 5};
        Instance().m_BlockIndices.at(ID::Bricks) = {7, 7, 7, 7, 7, 7};
        Instance().m_BlockIndices.at(ID::TNT) = {8, 8, 9, 10, 8, 8};
        Instance().m_BlockIndices.at(ID::Bookshelf) = {35, 35, 4, 4, 35, 35};
        Instance().m_BlockIndices.at(ID::Mossy_Cobblestone) = {36, 36, 36, 36, 36, 36};
        Instance().m_BlockIndices.at(ID::Obsidian) = {37, 37, 37, 37, 37, 37};
        Instance().m_BlockIndices.at(ID::Torch) = {80, 80, 80, 80, 80, 80};
        Instance().m_BlockIndices.at(ID::Fire) = {32, 32, 32, 32, 32, 32};
        Instance().m_BlockIndices.at(ID::Monster_Spawner) = {65, 65, 65, 65, 65, 65};
        Instance().m_BlockIndices.at(ID::Oak_Stairs) = {4, 4, 4, 4, 4, 4};
        Instance().m_BlockIndices.at(ID::Chest) = {27, 26, 25, 25, 26, 26};
        Instance().m_BlockIndices.at(ID::_Redstone) = {164, 164, 164, 164, 164, 164};
        Instance().m_BlockIndices.at(ID::Diamond_Ore) = {50, 50, 50, 50, 50, 50};
        Instance().m_BlockIndices.at(ID::Diamond_Block) = {24, 24, 24, 24, 24, 24};
        Instance().m_BlockIndices.at(ID::Crafting_Table) = {60, 60, 43, 4, 59, 59};
    }

    static std::array<unsigned char, 6> GetIndices(unsigned char blockID)
    {
        return Instance().m_BlockIndices.at(blockID);
    }

  private:
    BlockLibrary(){};
    BlockLibrary(BlockLibrary const &);
    void operator=(BlockLibrary const &);

    std::array<std::array<unsigned char, 6>, 256> m_BlockIndices;
};
