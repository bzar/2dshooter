#ifndef WORLD_H
#define WORLD_H

#include <set>
#include "../util/transformation.h"

class Entity;
#include "entity.h"

class World {
public:  
  World();
  ~World();
  
  void addEntity(Entity* const entity);
  void removeEntity(Entity* const entity);
  void detachEntity(Entity* const entity);
  
  void update(float delta);
  void render(Transformation const& view);
  
private:
  struct OrderByZIndex {
    bool operator() (Entity* a, Entity* b) const;
  };
  typedef std::set<Entity*, OrderByZIndex> EntitySet;
  EntitySet entities;
};

#endif
