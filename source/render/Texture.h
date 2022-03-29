#pragma once

#include "glad/gl.h"
#include "stb_image.h"

class Texture
{
public:
    Texture() {}
    Texture(const char *filepath) { Create(filepath); }
    ~Texture() {}

    void Create(const char *filepath)
    {
        // Texture Setup
        // stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filepath, &m_Width, &m_Height, &m_ChannelCount, 0);
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        // Repeat options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // No upscaling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void Delete()
    {
        m_Width = 0;
        m_Height = 0;
        m_ChannelCount = 0;
        glDeleteTextures(1, &m_ID);
    }

    void Bind() { glBindTexture(GL_TEXTURE_2D, m_ID); }
    void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

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