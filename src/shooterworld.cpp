#include "shooterworld.h"
#include "shooterentity.h"

ShooterWorld::ShooterWorld(Vec2D const& gravity) : World(), gravity(gravity), terrain()
{
  
}

void ShooterWorld::addEntity(ShooterEntity* const entity)
{
  entity->setWorld(this);
  World::addEntity(entity);
}

void ShooterWorld::addTerrain(SegmentGroup* const segmentGroup)
{
  terrain.insert(segmentGroup);
  addEntity(segmentGroup);
}

void ShooterWorld::removeTerrain(SegmentGroup* const segmentGroup)
{
  terrain.erase(segmentGroup);
  removeEntity(segmentGroup);
}
Vec2D const& ShooterWorld::getGravity() const
{
  return gravity;
}
void ShooterWorld::setGravity(Vec2D const& newGravity)
{
  gravity = newGravity;
}

std::set<SegmentGroup*> const& ShooterWorld::getTerrain() const
{
  return terrain;
}