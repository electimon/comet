#include "comet/Engine.h"

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
    world.SetShader(basic.GetID());

    Player player(glm::vec3(0.0f, 50.0f, 0.0f));
    player.SetWorld(&world);

    Engine::MainThread();

    world.EndWorldThread();

    return 0;
}