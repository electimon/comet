#pragma once

#include <vector>
#include <iostream>

#include "glfw/glfw3.h"
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
    void AllocateOnGPU();
    void DeallocateOnGPU();
    void Update() { m_TimeDelta = glfwGetTime() - m_TimeCreated; }
    double GetTimeDelta() { return m_TimeDelta; }
    float GetBrightness() { return m_TimeDelta; }

private:
    std::vector<Vertex> *p_Vertices;
    std::vector<unsigned int> *p_Indices;

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_IBO;
    unsigned int m_Shader;
    unsigned int m_Count;

    bool m_PushedToGPU;
    double m_TimeCreated;
    double m_TimeDelta;
    float m_AO;

public:
    glm::mat4 m_ModelMatrix;
    void SetShaderID(unsigned int id) { m_Shader = id; }
    unsigned int GetShaderID() { return m_Shader; }
    unsigned int GetCount() { return m_Count; }
    glm::mat4 GetModelMatrix() { return m_ModelMatrix; }
};
