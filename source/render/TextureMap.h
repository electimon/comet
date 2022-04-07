#pragma once

#include <vector>

#include <glm/glm.hpp>

class TextureMap {
public:
  static TextureMap &GetInstance() {
    static TextureMap s_Instance;
    return s_Instance;
  }

  // Singleton approach, assuming that if game is using a texture map
  // then game is probably only going to be loading in a single texture.

  static void Configure(int width, int height, int resolution);

  static std::vector<std::vector<float>> GetTextureCoordinates() {
    return GetInstance().m_Coords;
  }
  static glm::vec2 GetTopLeft(int index) {
    return {GetInstance().m_Coords[index][0], GetInstance().m_Coords[index][2]};
  }
  static glm::vec2 GetTopRight(int index) {
    return {GetInstance().m_Coords[index][1], GetInstance().m_Coords[index][2]};
  }
  static glm::vec2 GetBottomLeft(int index) {
    return {GetInstance().m_Coords[index][0], GetInstance().m_Coords[index][3]};
  }
  static glm::vec2 GetBottomRight(int index) {
    return {GetInstance().m_Coords[index][1], GetInstance().m_Coords[index][3]};
  }

private:
  TextureMap() {}
  TextureMap(TextureMap const &);
  void operator=(TextureMap const &) {}

  unsigned int m_Width = 0;      // horizontal pixel count
  unsigned int m_Height = 0;     // vertical pixel count
  unsigned int m_Resolution = 0; // resolution of each texture

  // vector of 4x1 vectors (left, right, bottom, top coordinates)
  std::vector<std::vector<float>> m_Coords;
};