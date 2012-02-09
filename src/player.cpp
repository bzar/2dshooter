#include "player.h"

Player::Player(Vec2D const& position) : Human(position)
{
  
}

void Player::moveRight()
{
  if(velocity.x < 5)
    velocity.x += 0.1;
}

void Player::moveLeft()
{
  if(velocity.x > -5)
    velocity.x -= 0.1;
}

void Player::stop()
{
  if(velocity.x > 0.4)
  {
    velocity.x -= 0.2;
  }
  else if(velocity.x < -0.4)
  {
    velocity.x += 0.2;
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