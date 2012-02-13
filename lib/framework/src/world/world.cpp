#include "world.h"
#include "../util/log.h"

World::World() : entities(), entitiesToDelete()
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
  return entity;
}

bool World::removeEntity(Entity* const entity)
{
  if(!detachEntity(entity))
    return false;
  
  delete entity;
  return true;
}

void World::removeEntityLater(Entity* const entity)
{
  entitiesToDelete.push_back(entity);
}

bool World::detachEntity(Entity* const entity)
{
  return entities.erase(entity) > 0;
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
  
  for(EntityList::iterator i = entitiesToDelete.begin(); i != entitiesToDelete.end(); ++i)
  {
    removeEntity(*i);
  }
  
  entitiesToDelete.clear();
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