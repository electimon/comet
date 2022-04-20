#include "Engine.h"

#include "interfaces/RenderMenu.h"

#include "entities/Player.h"

#include "world/Block.h"
#include "world/Chunk.h"
#include "world/World.h"

int main(void)
{
    // Initializing engine components
    Engine::Initialize();

    // Shader/Texture Setup
    ShaderProgram blockShader("../game/shaders/block.vert", "../game/shaders/block.frag");
    Texture texture("../game/textures/terrain.png");
    TextureMap::Configure(texture.Width(), texture.Height(), 16);



    // Starting world thread
    World::Initialize();
    World::SetSeed(1);
    World::SetShader(blockShader);
    World::SetRenderDistance(8);

    Player player;
    player.SetPosition({0.0f, 60.0f, 0.0f});
    Camera::SetPosition({0.0f, 60.0f, 0.0f});

    RenderMenu rendermenu;

    // Starting main engine thread
    World::Thread();
    Engine::Thread();


    // Ending threads
    World::Finalize();
    Engine::Finalize();

    return 0;
}