#ifndef BULLET_H
#define BULLET_H

#include "framework.h"
#include "movingentity.h"
#include "segmentgroup.h"

class Bullet : public MovingEntity
{
public:
  enum Owner { PLAYER, ENEMY };
  
  Bullet(Vec2D const& position, Vec2D const& velocity, float life, Owner const owner = PLAYER);
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
  float getLife() const;
  Owner getOwner() const;
  void setLife(float const newLife);
  void setIsRicochet(bool const newIsRicochet);
  void setOwner(Owner const newOwner);
  
private:
  class CollisionHandler : public SegmentGroup::ResultHandler
  {
  public:
    CollisionHandler(Bullet& bullet, float const timeDelta) : bullet(bullet), timeDelta(timeDelta) {}
    virtual bool handle(Segment const& segment, SegmentGroup const& group);
  private:
    Bullet& bullet;
    float timeDelta;
  };

  float life;
  bool isRicochet;
  Owner owner;
  Sprite sprite;
  
  static Image image;

};

#endif
