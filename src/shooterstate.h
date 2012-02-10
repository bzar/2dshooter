#ifndef SHOOTERSTATE_H
#define SHOOTERSTATE_H

#include "framework.h"
#include "shooterworld.h"

class Player;

class ShooterState : public Engine::State {
public:
  ShooterState();
  ~ShooterState();
  virtual void enter();
  virtual void exit();
  virtual void clear(Screen const& screen);
  virtual void update(Screen const& screen, float const& delta);
  virtual void draw(Screen const& screen);  
  
private:
  Player* player;
  ShooterWorld world;
  Transformation view;

  Segment newTerrainSegment;
  bool drawingTerrain;
};
#endif