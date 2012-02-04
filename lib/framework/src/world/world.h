#ifndef WORLD_H
#define WORLD_H

#include <set>
#include "../util/screen.h"

class World {
public:
  class Entity
  {
  public:
    Entity() {};
    void setWorld(World* const newWorld) { world = newWorld; };
    World* getWorld() const { return world; };
    virtual void render(Screen const& screen) {};
    virtual void intent(float const delta) {};
    virtual void reaction(float const delta) {};
    virtual void update(float const delta) {};

  private:
    World* world;
  };
  
  World();
  ~World();
  
  void addEntity(Entity* const entity);
  void removeEntity(Entity* const entity);
  
  void update(float delta);
  void render(Screen const& screen);
  
private:
  std::set<Entity*> entities;
};
#endif