#pragma once

#include <vector>
#include <iostream>

#include "glm/glm.hpp"
#include "glad/gl.h"

#include "Vertex.h"

class Mesh
{
public:
    Mesh();
    Mesh(std::vector<Vertex> *vertices, std::vector<unsigned int> *indices, unsigned int shader);
    ~Mesh();

    void Bind();
    // void AddGemoetry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int shader);

    bool IsPushedToGPU() { return m_PushedToGPU; }
    void PushToGPU();

private:
    std::vector<Vertex> *p_Vertices;
    std::vector<unsigned int> *p_Indices;

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_IBO;
    unsigned int m_Shader;
    unsigned int m_Count;

    bool m_PushedToGPU;

public:
    void SetShaderID(unsigned int id) { m_Shader = id; }
    unsigned int GetShaderID() { return m_Shader; }
    unsigned int GetCount() { return m_Count; }
};
