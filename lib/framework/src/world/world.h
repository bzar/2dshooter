#ifndef WORLD_H
#define WORLD_H

#include <set>
#include "../util/transformation.h"

class World {
public:
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
  
  World();
  ~World();
  
  void addEntity(Entity* const entity);
  void removeEntity(Entity* const entity);
  void detachEntity(Entity* const entity);
  
  void update(float delta);
  void render(Transformation const& view);
  
private:
  struct OrderByZIndex {
    bool operator() (Entity* a, Entity* b) const { return a->getZIndex() < b->getZIndex() || a < b; }
  };
  typedef std::set<Entity*, OrderByZIndex> EntitySet;
  EntitySet entities;
};
#endif