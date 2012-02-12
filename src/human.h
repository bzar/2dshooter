#ifndef HUMAN_H
#define HUMAN_H

#include "framework.h"
#include "movingentity.h"
#include "segmentgroup.h"
#include "bullet.h"

class Human : public MovingEntity
{
public:
  Human(Vec2D const& position);
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
  void setOnGround(bool const value);
  void aimAt(Vec2D const& point);
  void shoot(Bullet::Owner const owner = Bullet::ENEMY);
  
protected:
  bool onGround;
  
private:
  class CollisionHandler : public SegmentGroup::ResultHandler
  {
  public:
    CollisionHandler(Human& human) : human(human) {}
    virtual bool handle(Segment const& segment, SegmentGroup const& group) const;
  private:
    Human& human;
  };
  
  Sprite body;
  Sprite rifle;
  Sprite leftHand;
  Sprite rightHand;
  
  static Image bodyImage;
  static Image rifleImage;
  static Image leftHandImage;
  static Image rightHandImage;
};

#endif
