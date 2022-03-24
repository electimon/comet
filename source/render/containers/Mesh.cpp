#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> *vertices, std::vector<unsigned int> *indices, unsigned int shader)
    : p_Vertices(vertices),
      p_Indices(indices),
      m_Shader(shader),
      m_Count((unsigned int)(indices->size())),
      m_PushedToGPU(false)
{
}

Mesh::~Mesh()
{
    // std::cout << "Mesh::~Mesh()" << std::endl;
}

void Mesh::Bind()
{
    glUseProgram(m_Shader);
    glBindVertexArray(m_VAO);
}

void Mesh::AllocateOnGPU()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    // std::cout << "Generating VAO (id: " << m_VAO << ")" << std::endl;
    // std::cout << "Generating VBO (id: " << m_VBO << ")" << std::endl;
    // std::cout << "Generating IBO (id: " << m_IBO << ")" << std::endl;

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, p_Vertices->size() * sizeof(Vertex), p_Vertices->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_Indices->size() * sizeof(unsigned int), p_Indices->data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_PushedToGPU = true;
}

void Mesh::DeallocateOnGPU()
{
    // std::cout << "Deleting VAO (id: " << m_VAO << ")" << std::endl;
    // std::cout << "Deleting VBO (id: " << m_VBO << ")" << std::endl;
    // std::cout << "Deleting IBO (id: " << m_IBO << ")" << std::endl;

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);

    m_PushedToGPU = false;
}