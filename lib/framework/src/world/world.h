#ifndef WORLD_H
#define WORLD_H

#include <set>
#include "../util/screen.h"

class World {
public:
  class Entity
  {
  public:
    Entity() : world(0), zIndex(0) {};
    void setWorld(World* const newWorld) { world = newWorld; };
    World* getWorld() const { return world; };
    void setZIndex(int const newz) { zIndex = newz; };
    int getZIndex() const { return zIndex; };
    virtual void render(Screen const& screen) {};
    virtual void intent(float const delta) {};
    virtual void reaction(float const delta) {};
    virtual void update(float const delta) {};

  protected:
    World* world;
    int zIndex;
  };
  
  World();
  ~World();
  
  void addEntity(Entity* const entity);
  void removeEntity(Entity* const entity);
  
  void update(float delta);
  void render(Screen const& screen);
  
private:
  struct OrderByZIndex {
    bool operator() (Entity* a, Entity* b) const { return a->getZIndex() < b->getZIndex() ? true : a < b; }
  };
  typedef std::set<Entity*, OrderByZIndex> EntitySet;
  EntitySet entities;
};
#endif