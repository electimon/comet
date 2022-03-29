#include "TextureMap.h"

void TextureMap::LoadTexture(int width, int height, int resolution)
{
    auto &map = GetInstance();

    map.m_Width = width;
    map.m_Height = height;
    map.m_Resolution = resolution;

    for (int j = 0; j < map.m_Height; j += resolution)
    {
        for (int i = 0; i < map.m_Width; i += resolution)
        {
            float x = float(i);
            float y = float(j);

            float left = x / float(map.m_Width);
            float bottom = y / float(map.m_Height);

            float right = (x + float(resolution)) / float(map.m_Width);
            float top = (y + float(resolution)) / float(map.m_Height);

            std::vector<float> temp = {left, right, bottom, top};

            map.m_Coords.push_back(temp);
        }
    }
}