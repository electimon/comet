#include "World.h"

#include "world/BlockLibrary.h"
#include "world/Chunk.h"
#include "world/ChunkGenerator.h"

#include "entities/Player.h"

void World::InitializeThread()
{
    std::cout << "Initializing world thread...\n";

    Instance().m_Thread = std::thread(&World::Thread);
}

void World::Thread()
{
    World::Initialize();
    EntityHandler::Initialize();

    auto &world = Instance();

    while (!Engine::IsShouldClose() && !Renderer::IsResetting())
    {
        EntityHandler::Update();

        Generate();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Exiting world thread...\n";
}

void World::Initialize()
{
    std::filesystem::create_directory("world");

    ChunkGenerator::Initialize();
    BlockLibrary::Initialize();

    Instance().m_ChunkDataMap.clear();
    Instance().m_ChunkRenderMap.clear();
    Instance().m_ChunksToDelete.clear();
    Instance().m_ChunksToGenerate.clear();
    Instance().m_ChunksToRender.clear();
    Instance().m_ChunksToUnrender.clear();
}

void World::Finalize()
{
    std::cout << "Saving currently loaded chunks...\n";

    for (auto &chunk : Instance().m_ChunkDataMap)
    {
        chunk.second.~Chunk();
    }

    Instance().m_Thread.join();
}

Block World::GetBlock(const glm::ivec3 &worldPos)
{
    glm::ivec3 index = GetChunkIndex(worldPos);
    glm::ivec3 chunkCoord = GetChunkCoord(worldPos);

    if (Instance().m_ChunkDataMap.find(index) != Instance().m_ChunkDataMap.end())
    {
        return Instance().m_ChunkDataMap.at(index).GetBlock(chunkCoord);
    }
    else
    {
        return Block();
    }
}

void World::SetBlock(const glm::ivec3 &worldPos, Block blockToSet)
{
    if (worldPos.y > CHUNK_HEIGHT)
    {
        return;
    }

    glm::ivec3 index = GetChunkIndex(worldPos);
    glm::ivec3 chunkCoord = GetChunkCoord(worldPos);

    if (Instance().m_ChunkDataMap.find(index) != Instance().m_ChunkDataMap.end())
    {
        Block blockToReplace = Instance().m_ChunkDataMap.at(index).GetBlock(chunkCoord);

        Instance().m_ChunkDataMap.at(index).SetBlock({chunkCoord.x, chunkCoord.y, chunkCoord.z}, blockToSet);
        Instance().m_ChunkDataMap.at(index).GenerateMesh();
        Instance().m_ChunkDataMap.at(index).SetModified(true);

        // if (blockToSet.IsTransparent() && blockToReplace.IsTransparent())
        // {
        //     Renderer::UpdateMeshInQueue({index.x, index.y + 1, index.z});
        //     return;
        // }

        // if (!blockToSet.IsTransparent() && !blockToReplace.IsTransparent())
        // {
        //     Renderer::UpdateMeshInQueue(index);
        //     return;
        // }

        Renderer::UpdateMeshInQueue(index);
        Renderer::UpdateMeshInQueue({index.x, index.y + 1, index.z});

        return;
    }
    else
    {
        return;
    }
}

glm::ivec3 World::GetChunkCoord(const glm::ivec3 &worldPos)
{
    glm::ivec3 chunkIndex = GetChunkIndex(worldPos);
    glm::ivec3 chunkPos = worldPos;

    while (chunkPos.x < 0)
    {
        chunkPos.x += CHUNK_WIDTH;
    }
    while (chunkPos.x > CHUNK_WIDTH - 1)
    {
        chunkPos.x -= CHUNK_WIDTH;
    }
    while (chunkPos.z < 0)
    {
        chunkPos.z += CHUNK_WIDTH;
    }
    while (chunkPos.z > CHUNK_WIDTH - 1)
    {
        chunkPos.z -= CHUNK_WIDTH;
    }

    return chunkPos;
}

glm::ivec3 World::GetChunkIndex(const glm::ivec3 &worldPos)
{
    glm::ivec3 chunkIndex(0, 0, 0);

    chunkIndex.x = std::floor(static_cast<double>(worldPos.x) / static_cast<double>(CHUNK_WIDTH));
    chunkIndex.z = std::floor(static_cast<double>(worldPos.z) / static_cast<double>(CHUNK_WIDTH));

    return chunkIndex;
}

void World::ProcessRequestedChunks(const glm::ivec3 &centerChunkIndex)
{
    glm::ivec3 index;
    std::unordered_set<glm::ivec3> chunksGenerated;
    std::unordered_set<glm::ivec3> chunksRendered;
    int chunksToRenderAhead = 1;

    int lowerx = -chunksToRenderAhead + centerChunkIndex.x - Instance().m_RenderDistance;
    int lowerz = -chunksToRenderAhead + centerChunkIndex.z - Instance().m_RenderDistance;
    int upperx = 1 + chunksToRenderAhead + centerChunkIndex.x + Instance().m_RenderDistance;
    int upperz = 1 + chunksToRenderAhead + centerChunkIndex.z + Instance().m_RenderDistance;

    const auto &world = Instance();

    // Add chunks to generation
    for (int x = lowerx; x < upperx; x++)
    {
        for (int z = lowerz; z < upperz; z++)
        {
            index = {x, 0, z};
            chunksGenerated.insert(index);
            if (Instance().m_ChunkDataMap.find(index) == Instance().m_ChunkDataMap.end())
            {
                Instance().m_ChunksToGenerate.insert(index);
            }
        }
    }

    // Removes chunk data
    for (const auto &[index, chunk] : Instance().m_ChunkDataMap)
    {
        if (chunksGenerated.find(index) == chunksGenerated.end())
        {
            Instance().m_ChunksToDelete.insert(index);
        }
    }

    // Add chunks to render
    for (int x = lowerx + 1; x < upperx - 1; x++)
    {
        for (int z = lowerz + 1; z < upperz - 1; z++)
        {
            index = {x, 0, z};
            chunksRendered.insert(index);
            if (Instance().m_ChunkRenderMap.find(index) == Instance().m_ChunkRenderMap.end())
            {
                Instance().m_ChunksToRender.insert(index);
            }
        }
    }

    // Removes chunk geometry
    for (const auto &[index, chunk] : Instance().m_ChunkRenderMap)
    {
        if (chunksRendered.find(index) == chunksRendered.end())
        {
            Instance().m_ChunksToUnrender.insert(index);
        }
    }
}

void World::Generate()
{
    auto &world = Instance();

    // Generates chunks
    for (const auto &index : Instance().m_ChunksToGenerate)
    {
        // Generates chunk data
        // TODO: chunk destructor is called here, big memory issue here
        world.m_ChunkDataMap.insert_or_assign(index, Chunk(index));
        world.m_ChunkDataMap.at(index).Generate();
    }
    world.m_ChunksToGenerate.clear();

    // Renders chunk
    for (const auto &index : world.m_ChunksToRender)
    {
        // Generate Chunk Geometry
        if (world.m_ChunkDataMap.find(index) == world.m_ChunkDataMap.end())
            return;

        Chunk *chunk = &world.m_ChunkDataMap.at(index);
        chunk->GenerateMesh();
        world.m_ChunkRenderMap.insert_or_assign(index, chunk);

        Mesh solidMesh = Mesh(&chunk->SolidGeometry()->Vertices, &chunk->SolidGeometry()->Indices, &world.m_Shader);
        Mesh transparentMesh =
            Mesh(&chunk->TransparentGeometry()->Vertices, &chunk->TransparentGeometry()->Indices, &world.m_Shader);

        // Adding to Renderer
        Renderer::AddMeshToQueue(index, solidMesh);
        Renderer::AddMeshToQueue({index.x, index.y + 1, index.z}, transparentMesh);
    }
    world.m_ChunksToRender.clear();

    // Deletes chunks
    for (const auto &index : world.m_ChunksToDelete)
    {
        // remove chunk from data
        world.m_ChunkDataMap.erase(index);
    }
    world.m_ChunksToDelete.clear();

    // Unrenders chunk
    for (const auto &index : world.m_ChunksToUnrender)
    {
        // remove mesh from renderer
        Renderer::DeleteMeshFromQueue(index);
        Renderer::DeleteMeshFromQueue({index.x, index.y + 1, index.z});

        world.m_ChunkRenderMap.erase(index);
    }
    world.m_ChunksToUnrender.clear();
}