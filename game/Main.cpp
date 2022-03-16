#include "comet/Engine.h"
#include "render/Renderer.h"

#include "render/containers/Shader.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

int main(void)
{
    Engine *p_Engine = new Engine;
    p_Engine->Start();

    double startTime = glfwGetTime();

    World world;
    world.GenerateChunk(glm::ivec3(0, 0, 0));

    double timeTaken = glfwGetTime() - startTime;
    std::cout << "Chunk generated in: " << timeTaken << " seconds." << std::endl;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    std::vector<Vertex> v;
    v.resize(4);
    v[0].position = {-0.5f, -0.5f, 0.0f};
    v[1].position = {0.5f, -0.5f, 0.0f};
    v[2].position = {0.5f, 0.5f, 0.0f};
    v[3].position = {-0.5f, 0.5f, 0.0f};

    vertices.push_back(v[0]);
    vertices.push_back(v[1]);
    vertices.push_back(v[2]);
    vertices.push_back(v[3]);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";

    Mesh square(vertices, indices);
    Shader basic(vertexshader, fragshader);

    square.SetShaderID(basic.GetID());

    p_Engine->GetRenderer()->AddToMeshQueue(&square);

    p_Engine->MainLoop();

    return 0;
}