#include "TextureMap.h"

TextureMap::TextureMap(unsigned int width, unsigned int height, unsigned int resolution)
    : m_Width(width),
      m_Height(height),
      m_Resolution(resolution)
{
    for (unsigned int j = 0; j < m_Height; j++)
    {
        for (unsigned int i = 0; i < m_Width; i++)
        {
            float x = float(i);
            float y = float(j);
            float left = x / float(m_Width);
            float right = (x + 1.0f) / float(m_Width);
            float bottom = y / float(m_Height);
            float top = (y + 1.0f) / float(m_Height);

            std::vector<float> temp = {left, right, bottom, top};

            m_TextureCoordinates.push_back(temp);
        }
    }
}

TextureMap::~TextureMap()
{
}