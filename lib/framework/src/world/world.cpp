#include "world.h"

World::World() : entities()
{
  
}

World::~World()
{
  for(std::set<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    delete *i;
  }
  entities.clear();
}

void World::addEntity(Entity* const entity)
{
  entity->setWorld(this);
  entities.insert(entity);
}

void World::removeEntity(Entity* const entity)
{
  entities.erase(entity);
  delete entity;
}

void World::update(float delta)
{
  for(std::set<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->intent(delta);
  }
  for(std::set<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->reaction(delta);
  }
  for(std::set<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->update(delta);
  }
}

void World::render(Screen const& screen)
{
  for(std::set<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
  {
    (*i)->render(screen);
  }  
}
  