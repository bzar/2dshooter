#ifndef PLAYER_H
#define PLAYER_H

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
