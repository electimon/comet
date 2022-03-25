#pragma once

#include "FastNoise/FastNoise.h"

#include "glm/glm.hpp"

class ChunkDecorator
{
public:
    static ChunkDecorator &GetInstance()
    {
        static ChunkDecorator s_Instance;
        return s_Instance;
    }
    static void Initialize()
    {
        BiomeNoise = FastNoise::NewFromEncodedNodeTree("GwAJAAEHAA==");
    }

    static FastNoise::SmartNode<> BiomeNoise;

private:
    ChunkDecorator() {}
    ChunkDecorator(ChunkDecorator const &);
    void operator=(ChunkDecorator const &);


};