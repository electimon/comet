#pragma once

#include "FastNoiseLite.h"

class ChunkGenerator
{
public:
    static ChunkGenerator &GetInstance()
    {
        static ChunkGenerator s_Instance;
        return s_Instance;
    }
    static void Initialize()
    {
        GetInstance().m_SurfaceNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        GetInstance().m_SurfaceNoise.SetFrequency(0.02f);
        GetInstance().m_SurfaceNoise.SetSeed(1337);

        GetInstance().m_BiomeNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        GetInstance().m_BiomeNoise.SetFrequency(0.001f);
        GetInstance().m_BiomeNoise.SetSeed(1337);
    }
    static float GetSurfaceNoise(int x, int z)
    {
        return GetInstance().m_SurfaceNoise.GetNoise(static_cast<float>(x), static_cast<float>(z));
    }
    static float GetBiomeNoise(int x, int z)
    {
        return GetInstance().m_BiomeNoise.GetNoise(static_cast<float>(x), static_cast<float>(z));
    }

private:
    ChunkGenerator() {}
    ChunkGenerator(ChunkGenerator const &);
    void operator=(ChunkGenerator const &);

    FastNoiseLite m_SurfaceNoise;
    FastNoiseLite m_BiomeNoise;

};