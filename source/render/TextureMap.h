#pragma once

#include "glm/glm.hpp"
#include <vector>

class TextureMap
{
public:
    static TextureMap &GetInstance()
    {
        static TextureMap s_Instance;
        return s_Instance;
    }

    static void LoadTexture(int width, int height, int resolution);

    static std::vector<std::vector<float>> GetTextureCoordinates()
    {
        return GetInstance().m_TextureCoordinates;
    }
    static glm::vec2 GetTopLeft(int index)
    {
        return glm::vec2(GetInstance().m_TextureCoordinates[index][0], GetInstance().m_TextureCoordinates[index][3]);
    }
    static glm::vec2 GetTopRight(int index)
    {
        return glm::vec2(GetInstance().m_TextureCoordinates[index][1], GetInstance().m_TextureCoordinates[index][3]);
    }
    static glm::vec2 GetBottomLeft(int index)
    {
        return glm::vec2(GetInstance().m_TextureCoordinates[index][0], GetInstance().m_TextureCoordinates[index][2]);
    }
    static glm::vec2 GetBottomRight(int index)
    {
        return glm::vec2(GetInstance().m_TextureCoordinates[index][1], GetInstance().m_TextureCoordinates[index][2]);
    }

private:
    TextureMap() {}
    TextureMap(TextureMap const &);
    void operator=(TextureMap const &);

    unsigned int m_Width;      // how many textures there are horizontally
    unsigned int m_Height;     // how many textures there are vertically
    unsigned int m_Resolution; // the pixel resolution of each texture

    // vector of 4x1 vectors (left, right, bottom, top coordinates)
    std::vector<std::vector<float>> m_TextureCoordinates;
};