#pragma once

#include <iostream>
#include <unordered_set>

#include "Entity.h"

class EntityHandler {
public:
  static EntityHandler &GetInstance() {
    static EntityHandler s_Instance;
    return s_Instance;
  }

  static void AddEntityToQueue(Entity *entity);
  static void RemoveEntityFromQueue(Entity *entity);
  static void UpdateEntities();

private:
  EntityHandler() {}
  EntityHandler(EntityHandler const &);
  void operator=(EntityHandler const &);

  std::unordered_set<Entity *> m_Entities;
};