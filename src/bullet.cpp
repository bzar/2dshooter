#include "bullet.h"

ew::UID const Bullet::ID = ew::getUID();

Bullet::Bullet(GameWorld* world) : ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world), ew::VectorTerrainCollidable(world),
  world(world), o(nullptr), position(), velocity(), life(1.0), isRicochet(false)
{
  o = glhckCubeNew(0.5);
}

void Bullet::render(ew::RenderContext *context)
{
  glhckObjectColorb(o, isRicochet ? 128 : 255, isRicochet ? 128 : 255, isRicochet ? 128 : 255, 255);
  glhckObjectDraw(o);
}

void Bullet::update(const float delta)
{
  if(isRicochet)
  {
    velocity += world->getGravity() * delta;
  }
  position += velocity * delta;
  glhckObjectPositionf(o, position.x, position.y, 0);

  life -= delta;
  if(life <= 0)
  {
    world->removeEntity(this);
  }
}

void Bullet::collide(const ew::Collidable *other)
{
}

const Vec2D& Bullet::getPosition()
{
  return position;
}

const Vec2D& Bullet::getVelocity()
{
  return velocity;
}

void Bullet::setPosition(const Vec2D &newPosition)
{
  position = newPosition;
}

void Bullet::setVelocity(const Vec2D &newVelocity)
{
  velocity = newVelocity;
}

bool Bullet::vectorTerrainCollision(const Segment &segment, const Vec2D &collisionPoint, const float timeDelta)
{
  Vec2D segmentNormal = segment.delta().normal();
  Vec2D rest = position - collisionPoint;
  position += rest.projection(segmentNormal) * -2;
  velocity += velocity.projection(segmentNormal) * -2;
  isRicochet = true;
  velocity *= 0.8;
  return true;
}
