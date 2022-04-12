#include "Engine.h"

#include "entities/Player.h"

#include "world/Block.h"
#include "world/Chunk.h"
#include "world/World.h"

int main(void)
{
    Engine::Initialize();

    // Shader Setup
    Shader blockShader("../game/shaders/block.vert", "../game/shaders/block.frag");

    Texture texture("../game/textures/terrain.png");

    TextureMap::Configure(texture.GetWidth(), texture.GetHeight(), 16);

    World::Initialize();
    World::SetSeed(1);
    World::SetShader(blockShader);

    Player player(glm::vec3(-1.0f, 55.0f, -1.0f));

    Engine::MainThread();

    return 0;
}