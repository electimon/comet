#include "comet/Engine.h"
#include "render/Renderer.h"

#include "render/containers/Shader.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

int main(void)
{
    Engine::Initialize();

    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";
    Shader basic(vertexshader, fragshader);

    World world;
    for (int x = -4; x < 4; x++)
    {
        for (int z = -4; z < 4; z++)
        { // Generating a chunk
            Timer timer("chunk generation");
            world.GenerateChunk(x, 0, z);
            world.AddChunkToRenderQueue(x, 0, z);
            world.AddShaderToChunk(x, 0, z, basic);
        }
    }

    Engine::MainLoop();

    return 0;
}