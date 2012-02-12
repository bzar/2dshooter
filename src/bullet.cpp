#include "bullet.h"
#include "shooterworld.h"

Image Bullet::image("img/bullet.png", false);

Bullet::Bullet(Vec2D const& position, Vec2D const& velocity, float life, Owner const owner) :
  MovingEntity(position, velocity), life(life), owner(owner), sprite(image, position)
{
  Log::debug() << "Bullet " << position;
  image.load();
}

void Bullet::render(Transformation const& view)
{
  sprite.setPosition(position);
  sprite.render(view);
}

void Bullet::intent(float const delta)
{
  life -= delta;
}

void Bullet::reaction(float const delta)
{
  if(life <= 0)
  {
    getWorld()->removeEntityLater(this);
  }
}

void Bullet::update(float const delta)
{
  position += velocity.scale(delta);
}

float Bullet::getLife() const
{
  return life;
}

Bullet::Owner Bullet::getOwner() const
{
  return owner;
}

void Bullet::setLife(float const newLife)
{
  life = newLife;
}

void Bullet::setOwner(Owner const newOwner)
{
  owner = newOwner;
}
