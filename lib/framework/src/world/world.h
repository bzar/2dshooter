#ifndef WORLD_H
#define WORLD_H

#include <set>
#include <list>
#include "../util/transformation.h"

class Entity;
#include "entity.h"

class World {
public:  
  World();
  ~World();
  
  Entity* addEntity(Entity* const entity);
  bool removeEntity(Entity* const entity);
  void removeEntityLater(Entity* const entity);
  bool detachEntity(Entity* const entity);
  void removeAllEntities();
  
  void update(float delta);
  void render(Transformation const& view);
  
private:
  struct OrderByZIndex {
    bool operator() (Entity* a, Entity* b) const;
  };
  typedef std::set<Entity*, OrderByZIndex> EntitySet;
  typedef std::list<Entity*> EntityList;
  
  EntitySet entities;
  EntityList entitiesToDelete;
};

#endif
