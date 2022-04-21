#include "Engine.h"

#include "interfaces/RenderInterface.h"

#include "entities/Player.h"

#include "world/Block.h"
#include "world/Chunk.h"
#include "world/World.h"

int main(void)
{
    // Initializing engine components.
    // This is needed to initialize keyboard/mouse controls,
    // as well as the core OpenGL context.
    Engine::Initialize();

    // Shader/Texture Setup
    ShaderProgram blockShader("../game/shaders/block.vert", "../game/shaders/block.frag");
    Texture texture("../game/textures/terrain.png");
    TextureMap::Configure(texture.Width(), texture.Height(), 16);

    // Create the debugging menu
    RenderInterface renderInterface;

    // Starting world thread and configuring
    World::InitializeThread();
    World::SetSeed(1);
    World::SetShader(blockShader);
    World::SetRenderDistance(8);

    // Create player entity and add it to the entity handler
    // Entity handler is now on the world thread
    Player player;
    player.SetPosition({0.0f, 60.0f, 0.0f});
    Camera::SetPosition({0.0f, 60.0f, 0.0f});

    // Starting main engine thread
    Engine::Thread();

    // Ending threads
    World::Finalize();
    Engine::Finalize();

    return 0;
}