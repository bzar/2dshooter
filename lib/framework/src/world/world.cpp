#include "world.h"
#include "../util/log.h"

void World::Entity::setWorld(World* const newWorld) 
{ 
  world = newWorld; 
};

World* World::Entity::getWorld() const 
{ 
  return world; 
};

void World::Entity::setZIndex(int const newz) 
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

int World::Entity::getZIndex() const 
{ 
  return zIndex; 
};

World::World() : entities()
{
  
}

World::~World()
{
  for(EntitySet::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    delete *i;
  }
  entities.clear();
}

void World::addEntity(Entity* const entity)
{
  entity->setWorld(this);
  entities.insert(entity);
  Log::debug() << "New entity (total: " << static_cast<long unsigned int>(entities.size()) << ")";
}

void World::removeEntity(Entity* const entity)
{
  detachEntity(entity);
  delete entity;
  Log::debug() << "Removed entity (total: " << static_cast<long unsigned int>(entities.size()) << ")";
}

void World::detachEntity(Entity* const entity)
{
  entities.erase(entity);
}

void World::update(float delta)
{
  for(EntitySet::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->intent(delta);
  }
  for(EntitySet::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->reaction(delta);
  }
  for(EntitySet::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->update(delta);
  }
}

void World::render(Transformation const& view)
{
  for(EntitySet::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->render(view);
  }  
}
  