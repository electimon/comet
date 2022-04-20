#pragma once

#include <comet.pch>

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
    static void AddEntity(Entity *entity);
    static void RemoveEntity(Entity *entity);
    static void UpdateEntities();

  private:
    EntityHandler() {}
    EntityHandler(EntityHandler const &);
    void operator=(EntityHandler const &);

    std::unordered_set<Entity *> m_Entities;
};