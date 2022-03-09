#include "comet/Engine.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

int main(void)
{
    Engine* p_Engine = new Engine;

    p_Engine->Start();

    std::string file = "../game/objects/cube.blend";
    Assimp::Importer importer;
    importer.ReadFile(file, 0);

    return 0;
}