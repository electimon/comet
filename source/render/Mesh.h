#pragma once

#include <comet.pch>

#include "Shader.h"
#include "Vertex.h"

class Mesh
{
public:
    Mesh();
    Mesh(std::vector<Vertex> *vertices, std::vector<unsigned int> *indices, Shader *shader);
    ~Mesh();

    void Bind();
    void Unbind();

    void Initialize();
    void Update();
    void UpdateGeometry();
    void Finalize();

    bool IsPushedToGPU() { return m_PushedToGPU; }
    float GetTransparency() { return m_Transparency; }
    double GetTimeDelta() { return m_TimeDelta; }
    Shader *GetShader() { return m_Shader; }
    unsigned int GetCount() { return m_Count; }
    glm::mat4 GetModelMatrix() { return m_ModelMatrix; }

private:
    std::vector<Vertex> *p_Vertices;
    std::vector<unsigned int> *p_Indices;

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_IBO = 0;
    Shader *m_Shader = nullptr;
    unsigned int m_Count = 0;
    double m_Transparency = 0.0;

    glm::mat4 m_ModelMatrix = {0.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 0.0f};
    bool m_PushedToGPU = false;
    double m_TimeCreated = 0.0;
    double m_TimeDelta = 0.0;
};
