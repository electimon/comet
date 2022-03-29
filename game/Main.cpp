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
    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";
    Shader basic(vertexshader, fragshader);

    Texture texture;
    texture.Create("../game/textures/terrain.png");
    TextureMap::LoadTexture(texture.GetWidth(), texture.GetHeight(), 16);

    World world;
    world.SetShader(basic.GetID());

    Player player(glm::vec3(0.0f, 100.0f, 0.0f));
    player.SetWorld(&world);

    Engine::MainThread();

    world.EndWorldThread();

    return 0;
}