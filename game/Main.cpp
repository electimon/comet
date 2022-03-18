#include "comet/Engine.h"
#include "render/Renderer.h"

#include "render/containers/Shader.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

#include "glfw/glfw3.h"

int main(void)
{
    Engine::Start();

    World world;
    { // Generating a chunk
        double startTime = glfwGetTime();
        world.GenerateChunk(glm::ivec3(0, 0, 0));
        double timeTaken = glfwGetTime() - startTime;
        std::cout << "Chunk generated in: " << timeTaken << " seconds." << std::endl;
    }

    // Square for testing
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
    vertices.resize(4);
    vertices[0].position = {5.0f, 0.5f, -0.5f};
    vertices[1].position = {5.0f, -0.5f, -0.5f};
    vertices[2].position = {5.0f, -0.5f, 0.5f};
    vertices[3].position = {5.0f, 0.5f, 0.5f};
    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";
    Mesh square(vertices, indices);
    Shader basic(vertexshader, fragshader);
    square.SetShaderID(basic.GetID());
    Renderer::GetInstance().AddToMeshQueue(&square);


    Engine::MainLoop();

    return 0;
}