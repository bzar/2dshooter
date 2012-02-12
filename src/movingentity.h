#ifndef MOVING_ENTITY_H
#define MOVING_ENTITY_H

#include "framework.h"
#include "shooterentity.h"

class MovingEntity : public ShooterEntity
{
public:
  MovingEntity(Vec2D const& position, Vec2D const& velocity = Vec2D(0, 0));
  Vec2D getPosition() const;
  Vec2D getVelocity() const;
  void setPosition(Vec2D const& newPosition);
  void setVelocity(Vec2D const& newVelocity);
  
protected:
  Vec2D position;
  Vec2D velocity;
};

#endif
