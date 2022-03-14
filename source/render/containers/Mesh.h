#pragma once

#include <vector>

#include "glm/glm.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    {
        m_Vertices = vertices;
        m_Indices = indices;

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_IBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

        glBindVertexArray(0);
    }

    ~Mesh()
    {
        std::cout << "Mesh::~Mesh()" << std::endl;
        std::cout << "Deleting VAO (id: " << m_VAO << ")" << std::endl;
        glDeleteVertexArrays(1, &m_VAO);
        std::cout << "Deleting VBO (id: " << m_VBO << ")" << std::endl;
        glDeleteBuffers(1, &m_VBO);
        std::cout << "Deleting IBO (id: " << m_IBO << ")" << std::endl;
        glDeleteBuffers(1, &m_IBO);
    }

    void Bind()
    {
        glUseProgram(m_Shader);
        glBindVertexArray(m_VAO);
    }

    void AttachShader(unsigned int id)
    {
        m_Shader = id;
    }

    unsigned int GetCount()
    {
        return (unsigned int)m_Indices.size();
    }

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_IBO;
    unsigned int m_Shader;
};
