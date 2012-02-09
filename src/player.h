#ifndef PLAYER_HH
#define PLAYER_HH

#include "human.h"

class Player : public Human
{
public:
  Player(Vec2D const& position);
  void moveRight();
  void moveLeft();
  void stop();
  void jump();
  
private:
  
};

#endif
