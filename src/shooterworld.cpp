#include "shooterworld.h"
#include "shooterentity.h"
#include "level.h"

ShooterWorld::ShooterWorld(Vec2D const& gravity) : World(), gravity(gravity), terrain(), player(0)
{
  
}

void ShooterWorld::loadLevel(std::string const& filename)
{
  removeAllEntities();
  terrain.clear();
  
  Level level = loadLevelFromFile(filename);
  
  player = new Player(level.startPosition);
  player->setZIndex(1);
  addEntity(player);
  
  for(std::list<SegmentGroup*>::iterator i = level.segmentGroups.begin(); i != level.segmentGroups.end(); ++i)
  {
    addTerrain(*i);
  }
}

ShooterEntity* ShooterWorld::addEntity(ShooterEntity* const entity)
{
  entity->setWorld(this);
  World::addEntity(entity);
  return entity;
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

Player* ShooterWorld::getPlayer()
{
  return player;
}