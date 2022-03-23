#include "comet/Engine.h"
#include "handlers/WindowHandler.h"

#include <thread>

#include "Renderer.h"

#include "containers/Shader.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

#include "entities/Player.h"

#include <chrono>

void GameLoop();
unsigned int shader;

int main(void)
{
    Engine::Initialize();

    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";
    Shader basic(vertexshader, fragshader);
    shader = basic.GetID();

    std::thread GameThread(GameLoop);

    Engine::MainThread();

    return 0;
}

void GameLoop()
{
    World world;
    world.SetShader(shader);

    Player player(glm::vec3(0.0f, 50.0f, 0.0f));
    player.SetWorld(&world);

    while (!WindowHandler::ShouldWindowClose())
    {
        // player.Update();
        // std::cout << "Test thread at 10Hz" << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}