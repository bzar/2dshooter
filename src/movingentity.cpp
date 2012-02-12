#include "movingentity.h"

MovingEntity::MovingEntity(Vec2D const& position, Vec2D const& velocity) :
  position(position), velocity(velocity)
{
  
}
Vec2D MovingEntity::getPosition() const
{
  return position;
}
Vec2D MovingEntity::getVelocity() const
{
  return velocity;
}
void MovingEntity::setPosition(Vec2D const& newPosition)
{
  position = newPosition;
}
void MovingEntity::setVelocity(Vec2D const& newVelocity)
{
  velocity = newVelocity;
}

