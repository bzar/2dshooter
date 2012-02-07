#ifndef SHOOTERWORLD_H
#define SHOOTERWORLD_H

#include "framework.h"
#include <set>

class SegmentGroup;
#include "segmentgroup.h"

class ShooterEntity;
#include "shooterentity.h"

class ShooterWorld : public World
{
public:
  ShooterWorld(Vec2D const& gravity);
  void addEntity(ShooterEntity* const entity);
  void addTerrain(SegmentGroup* const segmentGroup);
  void removeTerrain(SegmentGroup* const segmentGroup);

  Vec2D const& getGravity() const;
  void setGravity(Vec2D const& newGravity);
  
  std::set<SegmentGroup*> const& getTerrain() const;
private:
  Vec2D gravity;
  std::set<SegmentGroup*> terrain;
};

#endif