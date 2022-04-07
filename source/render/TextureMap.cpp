#include "TextureMap.h"

void TextureMap::Configure(int width, int height, int resolution) {
  TextureMap &map = GetInstance();

  map.m_Width = width;
  map.m_Height = height;
  map.m_Resolution = resolution;

  float x, y, left, bottom, right, top;

  for (int j = 0; j < map.m_Height; j += resolution) {
    for (int i = 0; i < map.m_Width; i += resolution) {
      x = static_cast<float>(i);
      y = static_cast<float>(j);

      left = x / static_cast<float>(map.m_Width);
      bottom = y / static_cast<float>(map.m_Height);

      right = (x + static_cast<float>(resolution)) /
              static_cast<float>(map.m_Width);
      top = (y + static_cast<float>(resolution)) /
            static_cast<float>(map.m_Height);

      map.m_Coords.push_back({left, right, bottom, top});
    }
  }
}