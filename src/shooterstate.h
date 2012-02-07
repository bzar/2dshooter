#ifndef SHOOTERSTATE_H
#define SHOOTERSTATE_H

#include "framework.h"
#include "shooterworld.h"
#include "human.h"

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
  Human* player;
  ShooterWorld world;
  Transformation view;
};
#endif