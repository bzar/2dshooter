#include "entity.h"
#include "world.h"

void Entity::setWorld(World* const newWorld) 
{ 
  world = newWorld; 
};

World* Entity::getWorld() const 
{ 
  return world; 
};

void Entity::setZIndex(int const newz) 
{ 
  if(world)
  {
    world->detachEntity(this);
  }
  
  zIndex = newz; 
  
  if(world)
  {
    world->addEntity(this);
  }
};

int Entity::getZIndex() const 
{ 
  return zIndex; 
};

