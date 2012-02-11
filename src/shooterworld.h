#ifndef SHOOTERWORLD_H
#define SHOOTERWORLD_H

#include "framework.h"
#include <set>
#include <string>

class SegmentGroup;
#include "segmentgroup.h"

class Player;
#include "player.h"

class ShooterEntity;

class ShooterWorld : public World
{
public:
  ShooterWorld(Vec2D const& gravity);
  void loadLevel(std::string const& filename);
  ShooterEntity* addEntity(ShooterEntity* const entity);
  void addTerrain(SegmentGroup* const segmentGroup);
  void removeTerrain(SegmentGroup* const segmentGroup);

  Vec2D const& getGravity() const;
  void setGravity(Vec2D const& newGravity);
  
  std::set<SegmentGroup*> const& getTerrain() const;
  Player* getPlayer();
  
private:
  Vec2D gravity;
  std::set<SegmentGroup*> terrain;
  Player* player;
};

#endif