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
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int shader);
    ~Mesh();

    void Bind();
    void AddGemoetry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int shader);

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_IBO;
    unsigned int m_Shader;

public:
    void SetShaderID(unsigned int id) { m_Shader = id; }
    unsigned int GetShaderID() { return m_Shader; }
    unsigned int GetCount() { return (unsigned int)m_Indices.size(); }
};
