#ifndef SHOOTER_ENTITY_H
#define SHOOTER_ENTITY_H

#include "framework.h"

class ShooterWorld;

class ShooterEntity : public Entity
{
public:
  ShooterEntity();
  void setWorld(ShooterWorld* const newWorld);
  ShooterWorld* getWorld() const;

protected:
  ShooterWorld* shooterWorld;
};


#endif