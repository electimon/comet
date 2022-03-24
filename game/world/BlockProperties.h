#pragma once

#include <vector>

#include "glm/glm.hpp"

std::vector<glm::vec4> BlockColor = {
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),                                  // 0 Debug
    glm::vec4(94.0f / 255.0f, 94.0f / 255.0f, 94.0f / 255.0f, 1.0f),    // 1 Stone
    glm::vec4(26.0f / 255.0f, 166.0f / 255.0f, 18.0f / 255.0f, 1.0f),   // 2 Grass
    glm::vec4(92.0f / 255.0f, 68.0f / 255.0f, 46.0f / 255.0f, 1.0f),    // 3 Dirt
    glm::vec4(66.0f / 255.0f, 173.0f / 255.0f, 245.0f / 255.0f, 0.5f),  // 4 Water
    glm::vec4(110.0f / 255.0f, 78.0f / 255.0f, 48.0f / 255.0f, 1.0f),   // 5 Log
    glm::vec4(42.0f / 255.0f, 117.0f / 255.0f, 9.0f / 255.0f, 1.0f),    // 6 Leaves
    glm::vec4(214.0f / 255.0f, 214.0f / 255.0f, 135.0f / 255.0f, 1.0f), // 7 Sand
};