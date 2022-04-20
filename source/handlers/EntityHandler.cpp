#include "EntityHandler.h"

void EntityHandler::Initialize() { Instance(); }

void EntityHandler::AddEntity(Entity *entity) { Instance().m_Entities.insert(entity); }

void EntityHandler::RemoveEntity(Entity *entity) { Instance().m_Entities.erase(entity); }

void EntityHandler::UpdateEntities()
{
    for (const auto &entity : Instance().m_Entities)
    {
        entity->Update();
    }
}