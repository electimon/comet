#include "comet/Engine.h"
#include "render/Renderer.h"

#include "render/containers/Shader.h"

int main(void)
{
    Engine* p_Engine = new Engine;
    p_Engine->Start();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Vertex v1, v2, v3;
    v1.position = {-0.5f, -0.5f, 0.0f};
    v1.color = {1.0f, 0.0f, 0.0f};
    v2.position = {0.5f, -0.5f, 0.0f};
    v2.color = {0.0f, 1.0f, 0.0f};
    v3.position = {0.0f, 0.5f, 0.0f};
    v3.color = {0.0f, 0.0f, 1.0f};
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    Shader basic;
    basic.CompileShaders("../source/render/shaders/basic.vert", "../source/render/shaders/basic.frag");

    Mesh triangle(vertices, indices);
    triangle.AttachShader(basic.GetID());

    p_Engine->GetRenderer()->AddToMeshQueue(&triangle);

    p_Engine->MainLoop();

    return 0;
}