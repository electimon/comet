#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "glad/gl.h"

class Shader
{
public:
    Shader(const char *vertFile, const char *fragFile);
    ~Shader();

    void Bind();
    void Unbind();
    unsigned int GetID();

    int GetUniformLocation(const std::string &name);

private:
    unsigned int m_ID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};