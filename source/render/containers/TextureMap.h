#pragma once

#include "glm/glm.hpp"
#include <vector>

class TextureMap
{
public:
    TextureMap(unsigned int width, unsigned int height, unsigned int resolution);
    ~TextureMap();

    void SetTextureMapWidth(unsigned int width) { m_Width = width; }
    void SetTextureMapHeight(unsigned int height) { m_Height = height; }
    void SetTextureMapResolution(unsigned int resolution) { m_Resolution = resolution; }

    std::vector<std::vector<float>> GetTextureCoordinates() { return m_TextureCoordinates; }

private:
    unsigned int m_Width;      // how many textures there are horizontally
    unsigned int m_Height;     // how many textures there are vertically
    unsigned int m_Resolution; // the pixel resolution of each texture

    // vector of 4x1 vectors (left, right, bottom, top coordinates)
    std::vector<std::vector<float>> m_TextureCoordinates;
};