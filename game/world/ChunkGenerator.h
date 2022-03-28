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
        GetInstance().m_ChunkHeight = 512;
        GetInstance().m_ChunkSize = 16;

        GetInstance().m_MediumNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        GetInstance().m_MediumNoise.SetFrequency(0.01f);
        GetInstance().m_MediumNoise.SetSeed(1337);

        GetInstance().m_SlowNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        GetInstance().m_SlowNoise.SetFrequency(0.001f);
        GetInstance().m_SlowNoise.SetSeed(1337);

        GetInstance().m_CellularNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
        GetInstance().m_CellularNoise.SetFrequency(0.01f);
        GetInstance().m_CellularNoise.SetSeed(1337);

        GetInstance().m_FastNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        GetInstance().m_FastNoise.SetFrequency(1.0f);
        GetInstance().m_FastNoise.SetSeed(1337);

        GetInstance().m_MediumChaotic.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        GetInstance().m_MediumChaotic.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
        GetInstance().m_MediumChaotic.SetFractalOctaves(4);
        GetInstance().m_MediumChaotic.SetFractalLacunarity(2.00f);
        // GetInstance().m_MediumChaotic.SetFractalGain(0.5f);
        GetInstance().m_MediumChaotic.SetFrequency(0.01f);

        GetInstance().m_BiomeNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
        GetInstance().m_BiomeNoise.SetFrequency(0.015f);
        GetInstance().m_BiomeNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Hybrid);
        GetInstance().m_BiomeNoise.SetCellularReturnType(FastNoiseLite::CellularReturnType::CellularReturnType_CellValue);
        GetInstance().m_BiomeNoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
        GetInstance().m_BiomeNoise.SetDomainWarpAmp(50.0f);
    }
    static float GetBiomeNoise(int x, int z)
    {
        return GetInstance().m_BiomeNoise.GetNoise(static_cast<float>(x), static_cast<float>(z));
    }
    static float GetMediumChaotic(int x, int z)
    {
        return GetInstance().m_MediumChaotic.GetNoise(static_cast<float>(x), static_cast<float>(z));
    }
    static float GetMediumNoise(int x, int z)
    {
        return GetInstance().m_MediumNoise.GetNoise(static_cast<float>(x), static_cast<float>(z));
    }
    static float GetSlowNoise(int x, int z)
    {
        return GetInstance().m_SlowNoise.GetNoise(static_cast<float>(x), static_cast<float>(z));
    }
    static float GetCellularNoise(int x, int y)
    {
        return GetInstance().m_CellularNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
    }
    static float GetFastNoise(int x, int z)
    {
        return GetInstance().m_FastNoise.GetNoise(static_cast<float>(x), static_cast<float>(z));
    }

    static void GenerateChunk(const glm::ivec3 &chunkIndex, const std::vector<unsigned char> *blockData)
    {
        // move all chunk generation into here eventually
    }

private:
    ChunkGenerator() {}
    ChunkGenerator(ChunkGenerator const &);
    void operator=(ChunkGenerator const &);

    FastNoiseLite m_MediumNoise;
    FastNoiseLite m_SlowNoise;
    FastNoiseLite m_CellularNoise;
    FastNoiseLite m_FastNoise;
    FastNoiseLite m_MediumChaotic;
    FastNoiseLite m_BiomeNoise;

    glm::ivec3 m_ChunkIndex;
    int m_ChunkHeight;
    int m_ChunkSize;
};