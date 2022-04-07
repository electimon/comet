#include "Engine.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

#include "entities/Player.h"

int main(void)
{
    Engine::Initialize();

    // Shader Setup
    Shader blockShader("../game/shaders/block.vert",
                       "../game/shaders/block.frag");

    Texture texture("../game/textures/terrain.png");

    TextureMap::Configure(texture.GetWidth(), texture.GetHeight(), 16);

    World::Initialize();
    World::SetSeed(1);
    World::SetShader(blockShader);

    Player player(glm::vec3(0.0f, 60.0f, 0.0f));

    Engine::MainThread();

    return 0;
}