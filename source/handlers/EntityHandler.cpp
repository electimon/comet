#include "EntityHandler.h"

void EntityHandler::AddEntityToQueue(Entity *entity)
{
    GetInstance().m_Entities.insert(entity);
}

void EntityHandler::RemoveEntityFromQueue(Entity *entity)
{
    GetInstance().m_Entities.erase(entity);
}

void EntityHandler::UpdateEntities()
{
    for (const auto &entity : GetInstance().m_Entities)
    {
        entity->Update();
    }
}