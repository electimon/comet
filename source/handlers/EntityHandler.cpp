#include "EntityHandler.h"

void EntityHandler::AddEntityToQueue(Entity *entity)
{
    Get().m_Entities.insert(entity);
}

void EntityHandler::RemoveEntityFromQueue(Entity *entity)
{
    Get().m_Entities.erase(entity);
}

void EntityHandler::UpdateEntities()
{
    for (const auto &entity : Get().m_Entities)
    {
        entity->Update();
    }
}