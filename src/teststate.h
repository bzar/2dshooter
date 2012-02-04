#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "framework.h"

class TestState : public Engine::State {
public:
  TestState();
  ~TestState();
  virtual void enter();
  virtual void exit();
  virtual void clear(Screen const& screen);
  virtual void update(Screen const& screen, float const& delta);
  virtual void draw(Screen const& screen);  
  
private:
  World world;
};
#endif