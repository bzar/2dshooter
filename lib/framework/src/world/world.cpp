#include "world.h"
#include "../util/log.h"

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
  entities.erase(entity);
  delete entity;
  Log::debug() << "Removed entity (total: " << static_cast<long unsigned int>(entities.size()) << ")";
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

void World::render(Screen const& screen)
{
  for(EntitySet::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->render(screen);
  }  
}
  