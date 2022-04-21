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

    static void AddToUpdater(Entity *entity);
    static void RemoveFromUpdater(Entity *entity);

    static void AddToFrameUpdater(Entity *entity);
    static void RemoveFromFrameUpdater(Entity *entity);

    static void Update();
    static void FrameUpdate();

  private:
    EntityHandler() {}
    EntityHandler(EntityHandler const &);
    void operator=(EntityHandler const &);

    std::unordered_set<Entity *> m_Updates;
    std::unordered_set<Entity *> m_FrameUpdates;
};