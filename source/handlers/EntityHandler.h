#pragma once

#include <iostream>
#include <unordered_set>

#include "Entity.h"

class EntityHandler
{
public:
    inline static auto &Instance()
    {
        static EntityHandler instance;
        return instance;
    }

    static void Initialize();
    static void AddEntityToQueue(Entity *entity);
    static void RemoveEntityFromQueue(Entity *entity);
    static void UpdateEntities();

private:
    EntityHandler() {}
    EntityHandler(EntityHandler const &);
    void operator=(EntityHandler const &);

    std::unordered_set<Entity *> m_Entities;
};