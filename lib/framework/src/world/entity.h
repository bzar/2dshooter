#ifndef ENTITY_H
#define ENTITY_H

#include "../util/transformation.h"

class World;
#include "world.h"

class Entity
{
public:
  Entity() : world(0), zIndex(0) {};
  void setWorld(World* const newWorld);
  World* getWorld() const;
  void setZIndex(int const newz);
  int getZIndex() const;
  virtual void render(Transformation const& view) {};
  virtual void intent(float const delta) {};
  virtual void reaction(float const delta) {};
  virtual void update(float const delta) {};

protected:
  World* world;
  int zIndex;
};

#endif