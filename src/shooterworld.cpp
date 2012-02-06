#include "shooterworld.h"

ShooterWorld::ShooterWorld(Vec2D const& gravity) : World(), gravity(gravity)
{
  
}

Vec2D const& ShooterWorld::getGravity() const
{
  return gravity;
}
void ShooterWorld::setGravity(Vec2D const& newGravity)
{
  gravity = newGravity;
}
