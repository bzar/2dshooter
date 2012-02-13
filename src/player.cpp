#include "player.h"

Player::Player(Vec2D const& position) : Human(position)
{
  
}

void Player::moveRight()
{
  if(velocity.x < 200)
    velocity.x += 20;
}

void Player::moveLeft()
{
  if(velocity.x > -200)
    velocity.x -= 20;
}

void Player::stop()
{
  if(velocity.x > 40)
  {
    velocity.x -= 40;
  }
  else if(velocity.x < -40)
  {
    velocity.x += 40;
  }
  else
  {
    velocity.x = 0;
  }
}

void Player::jump()
{
  if(onGround)
    velocity.y += 370;
}