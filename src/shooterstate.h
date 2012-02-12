#ifndef SHOOTERSTATE_H
#define SHOOTERSTATE_H

#include "framework.h"
#include "shooterworld.h"
#include "input.h"

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
  static float const GRID_SIZE = 16; 
  
  float snapToGrid(float const value) const;
  Vec2D snapToGrid(Vec2D const& value) const;

  ShooterWorld world;
  Input input;
  
  Transformation view;
  Vec2D viewCenter;
  
  Segment newTerrainSegment;
  bool drawingTerrain;
};
#endif