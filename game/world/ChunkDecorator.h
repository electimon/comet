#pragma once

#include "FastNoise/FastNoise.h"
#include "FastNoise/SmartNode.h"

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
        GetInstance().SurfaceNoise = FastNoise::NewFromEncodedNodeTree("GwAJAAEHAA==");
    }

    static const FastNoise::SmartNode<> &GetSurfaceNoise()
    {
        return GetInstance().SurfaceNoise;
    }

private:
    ChunkDecorator() {}
    ChunkDecorator(ChunkDecorator const &);
    void operator=(ChunkDecorator const &);

    FastNoise::SmartNode<> SurfaceNoise;
};