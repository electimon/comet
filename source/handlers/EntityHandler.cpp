#include "EntityHandler.h"

void EntityHandler::Initialize() { Instance(); }

void EntityHandler::AddEntityToQueue(Entity *entity) { Instance().m_Entities.insert(entity); }

void EntityHandler::RemoveEntityFromQueue(Entity *entity) { Instance().m_Entities.erase(entity); }

void EntityHandler::UpdateEntities()
{
    for (const auto &entity : Instance().m_Entities)
    {
        entity->Update();
    }
}