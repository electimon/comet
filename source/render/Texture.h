#pragma once

#include "glad/gl.h"
#include "stb_image.h"

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

    int GetWidth() { return m_Width; }
    int GetHeight() { return m_Height; }
    int GetChannelCount() { return m_ChannelCount; }
    unsigned int GetID() { return m_ID; }

private:
    int m_Width;
    int m_Height;
    int m_ChannelCount;
    unsigned int m_ID;
};