#pragma once

#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>

#include "Vertex.h"
#include "Shader.h"

class Mesh
{
public:
    Mesh();
    Mesh(std::vector<Vertex> *vertices, std::vector<unsigned int> *indices, const Shader &shader);
    ~Mesh();

    void Bind();
    void Unbind();

    void Initialize();
    void Update();
    void UpdateGeometry();
    void Finalize();

    bool IsPushedToGPU() { return m_PushedToGPU; }
    float GetTransparency() { return m_TimeDelta; }
    double GetTimeDelta() { return m_TimeDelta; }
    Shader GetShader() { return m_Shader; }
    unsigned int GetCount() { return m_Count; }
    glm::mat4 GetModelMatrix() { return m_ModelMatrix; }

private:
    std::vector<Vertex> *p_Vertices;
    std::vector<unsigned int> *p_Indices;

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_IBO;
    Shader m_Shader;
    unsigned int m_Count;
    double m_Transparency;

    glm::mat4 m_ModelMatrix;
    bool m_PushedToGPU;
    double m_TimeCreated;
    double m_TimeDelta;
};
