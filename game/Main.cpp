#include "comet/Engine.h"
#include "Renderer.h"

#include "containers/Shader.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

#include "entities/Player.h"

int main(void)
{
    Engine::Initialize();

    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";
    Shader basic(vertexshader, fragshader);


    World world;
    Player player(glm::vec3(0.0f, 50.0f, 0.0f));
    player.SetWorld(&world);


    Engine::MainLoop();

    return 0;
}