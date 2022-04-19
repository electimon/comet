#include "Engine.h"

#include "entities/Player.h"

#include "world/Block.h"
#include "world/Chunk.h"
#include "world/World.h"

int main(void)
{
    Engine::Initialize();

    // Shader Setup
    ShaderProgram blockShader("../game/shaders/block.vert",
                              "../game/shaders/block.frag");

    Texture texture("../game/textures/terrain.png");

    TextureMap::Configure(texture.Width(), texture.Height(), 16);

    World::Initialize();
    World::SetSeed(1);
    World::SetShader(blockShader);

    Player player;
    player.SetPosition({0.0f, 60.0f, 0.0f});
    Camera::SetPosition({0.0f, 60.0f, 0.0f});
    player.SetRenderDistance(8);

    Engine::MainThread();

    return 0;
}