#include "comet/Engine.h"

#include "TextureMap.h"
#include "containers/Shader.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

#include "entities/Player.h"

#include "stb_image.h"

int main(void)
{
    Engine::Initialize();

    // Shader Setup
    char vertexshader[] = "../source/render/shaders/basic.vert";
    char fragshader[] = "../source/render/shaders/basic.frag";
    Shader basic(vertexshader, fragshader);

    // Texture Setup
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../game/textures/terrain.png", &width, &height, &nrChannels, 0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // Repeat options~
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // No upscaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    TextureMap::LoadTexture(width, height, 16);

    // glm::vec2 test = TextureMap::GetTopLeft(1);

    World world;
    world.SetShader(basic.GetID());

    Player player(glm::vec3(0.0f, 50.0f, 0.0f));
    player.SetWorld(&world);

    Engine::MainThread();

    world.EndWorldThread();

    return 0;
}