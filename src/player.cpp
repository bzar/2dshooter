#include "player.h"

Player::Player(Vec2D const& position) : Human(position)
{
  
}

void Player::moveRight()
{
  if(velocity.x < 3)
    velocity.x += 0.2;
}

void Player::moveLeft()
{
  if(velocity.x > -3)
    velocity.x -= 0.2;
}

void Player::stop()
{
  if(velocity.x > 0.4)
  {
    velocity.x -= 0.4;
  }
  else if(velocity.x < -0.4)
  {
    velocity.x += 0.4;
  }
  else
  {
    velocity.x = 0;
  }
}

void Player::jump()
{
  if(onGround)
    velocity.y += 5;
}