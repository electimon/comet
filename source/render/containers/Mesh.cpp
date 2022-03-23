#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int shader)
{
    AddGemoetry(vertices, indices, shader);
}

Mesh::~Mesh()
{
    std::cout << "Mesh::~Mesh()" << std::endl;

    std::cout << "Deleting VAO (id: " << m_VAO << ")" << std::endl;
    std::cout << "Deleting VBO (id: " << m_VBO << ")" << std::endl;
    std::cout << "Deleting IBO (id: " << m_IBO << ")" << std::endl;

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
}

void Mesh::Bind()
{
    glUseProgram(m_Shader);
    glBindVertexArray(m_VAO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void Mesh::AddGemoetry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int shader)
{
    m_Vertices = vertices;
    m_Indices = indices;
    m_Shader = shader;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    std::cout << "Generating VAO (id: " << m_VAO << ")" << std::endl;
    std::cout << "Generating VBO (id: " << m_VBO << ")" << std::endl;
    std::cout << "Generating IBO (id: " << m_IBO << ")" << std::endl;

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}