#ifndef SHOOTERWORLD_H
#define SHOOTERWORLD_H

#include "framework.h"

class ShooterWorld : public World
{
public:
    class ShooterEntity : public World::Entity
    {
    public:
      ShooterEntity() : Entity(), shooterWorld(0) {};
      void setWorld(ShooterWorld* const newWorld) { shooterWorld = newWorld; };
      ShooterWorld* getWorld() const { return shooterWorld; };

    protected:
      ShooterWorld* shooterWorld;
    };

  ShooterWorld(Vec2D const& gravity);
  void addEntity(ShooterEntity* const entity);

  Vec2D const& getGravity() const;
  void setGravity(Vec2D const& newGravity);
  
private:
  Vec2D gravity;
};

#endif