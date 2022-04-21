#pragma once

#include <comet.pch>
#include <glad/gl.h>

class Texture
{
  public:
    Texture();
    Texture(const char *filepath);
    ~Texture();

    void Bind();
    void Unbind();

    void Create(const char *filepath);
    void Delete();

  private:
    int m_Width;
    int m_Height;
    int m_ChannelCount;
    unsigned int m_ID;

  public:
    int Width() const { return m_Width; }
    void SetWidth(int Width) { m_Width = Width; }

    int Height() const { return m_Height; }
    void SetHeight(int Height) { m_Height = Height; }

    int ChannelCount() const { return m_ChannelCount; }
    void SetChannelCount(int ChannelCount) { m_ChannelCount = ChannelCount; }

    unsigned int ID() const { return m_ID; }
};
