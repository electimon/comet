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

    {
        Timer timer("chunk generation");

        World world;
        for (int x = -8; x < 8; x++)
        {
            for (int z = -8; z < 8; z++)
            { // Generating a chunk
                world.GenerateChunk(x, 0, z);
                world.AddChunkToRenderQueue(x, 0, z);
                world.AddShaderToChunk(x, 0, z, basic);
            }
        }
    }

    Engine::MainLoop();

    return 0;
}