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

    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";
    Shader basic(vertexshader, fragshader);

    World world;
    { // Generating a chunk
        double startTime = glfwGetTime();
        world.GenerateChunk(0, 0, 0);
        world.AddChunkToRenderQueue(0, 0, 0);
        world.AddShaderToChunk(0, 0, 0, basic);
        std::cout << "Chunk generated in: " << glfwGetTime() - startTime << " seconds." << std::endl;
    }

    // Square for testing
    // std::vector<Vertex> vertices;
    // std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
    // vertices.resize(4);
    // vertices[0].position = {5.0f, 0.5f, -0.5f};
    // vertices[1].position = {5.0f, -0.5f, -0.5f};
    // vertices[2].position = {5.0f, -0.5f, 0.5f};
    // vertices[3].position = {5.0f, 0.5f, 0.5f};
    // Mesh square(vertices, indices);
    // square.SetShaderID(basic.GetID());
    // Renderer::AddToMeshQueue(&square);

    Engine::MainLoop();

    return 0;
}