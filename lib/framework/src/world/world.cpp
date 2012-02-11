#include "world.h"
#include "../util/log.h"

World::World() : entities()
{
  
}

World::~World()
{
  removeAllEntities();
}

Entity* World::addEntity(Entity* const entity)
{
  entity->setWorld(this);
  entities.insert(entity);
  Log::debug() << "New entity (total: " << static_cast<long unsigned int>(entities.size()) << ")";
  return entity;
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

void World::removeAllEntities()
{
  for(EntitySet::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    removeEntity(*i);
  }
  entities.clear();
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

bool World::OrderByZIndex::operator()(Entity* a, Entity* b) const
{ 
  return a->getZIndex() < b->getZIndex() || a < b; 
}