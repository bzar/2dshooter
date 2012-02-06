#include "shooterworld.h"

ShooterWorld::ShooterWorld(Vec2D const& gravity) : World(), gravity(gravity)
{
  
}

void ShooterWorld::addEntity(ShooterEntity* const entity)
{
  entity->setWorld(this);
  World::addEntity(entity);
}

Vec2D const& ShooterWorld::getGravity() const
{
  return gravity;
}
void ShooterWorld::setGravity(Vec2D const& newGravity)
{
  gravity = newGravity;
}
