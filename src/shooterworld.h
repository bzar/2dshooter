#ifndef SHOOTERWORLD_H
#define SHOOTERWORLD_H

#include "framework.h"

class ShooterWorld : public World
{
public:
  ShooterWorld(Vec2D const& gravity);
  
  Vec2D const& getGravity() const;
  void setGravity(Vec2D const& newGravity);
  
private:
  Vec2D gravity;
};

#endif