#include "comet/Engine.h"

#include "TextureMap.h"
#include "Texture.h"
#include "containers/Shader.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

#include "entities/Player.h"

int main(void)
{
    Engine::Initialize();

    // Shader Setup
    Shader basic("../source/render/shaders/basic.vert",
                 "../source/render/shaders/basic.frag");

    Texture texture("../game/textures/terrain.png");

    TextureMap::Configure(texture.GetWidth(), texture.GetHeight(), 16);

    World world;
    world.SetShader(basic);

    Player player(glm::vec3(0.0f, 100.0f, 0.0f));
    player.SetWorld(&world);

    Engine::MainThread();

    return 0;
}