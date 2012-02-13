#include "bullet.h"
#include "shooterworld.h"

Image Bullet::image("img/bullet.png", false);

Bullet::Bullet(Vec2D const& position, Vec2D const& velocity, float life, Owner const owner) :
  MovingEntity(position, velocity), life(life), isRicochet(false), owner(owner), sprite(image, position)
{
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
  if(isRicochet)
    velocity += getWorld()->getGravity().scale(delta);
}

void Bullet::reaction(float const delta)
{
  if(life <= 0)
  {
    getWorld()->removeEntityLater(this);
  }
  else
  {
    std::set<SegmentGroup*> const& terrain = getWorld()->getTerrain();
    CollisionHandler handler(*this, delta);
    Segment s(position, position + velocity);
    
    for(std::set<SegmentGroup*>::const_iterator i = terrain.begin(); i != terrain.end(); ++i)
    {
      (*i)->query(s, handler);
    }
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

void Bullet::setIsRicochet(bool const newIsRicochet)
{
  isRicochet = newIsRicochet;
}

bool Bullet::CollisionHandler::handle(Segment const& segment, SegmentGroup const& group)
{
  Vec2D relativeVelocity = (bullet.getVelocity() - group.getVelocity()).scalei(timeDelta);
  Segment relativeMotion(bullet.getPosition(), bullet.getPosition() + relativeVelocity);
  if(segment.intersects(relativeMotion))
  {
    Vec2D segmentDelta = segment.delta();
    if(segmentDelta.cross(relativeVelocity) < 0)
    {
      Vec2D collisionPoint = segment.intersectionPoint(relativeMotion);
      Vec2D segmentNormal = segmentDelta.normal().uniti();
      bullet.setIsRicochet(true);
      bullet.setPosition(collisionPoint);
      bullet.setVelocity(bullet.getVelocity().subtracti(bullet.getVelocity().projection(segmentNormal).scale(1.5)).scalei(0.5));
      return true;
      /*
      
      
      Vec2D tox = collisionPoint - human.getPosition();
      Vec2D rest = human.getPosition() + human.velocity - collisionPoint;

      human.setVelocity(tox.addi(rest.projectioni(segmentDelta))
                        .subtracti(segmentNormal.scale(0.1))
                        .addi(group.getVelocity()));
      
      if(segmentNormal.dot(human.shooterWorld->getGravity().unit()) > 0.5) {
        human.setOnGround(true);
      }
      
      return true;
    }*/
    }
  }
  return false;
}