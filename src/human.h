#ifndef HUMAN_HH
#define HUMAN_HH

#include "framework.h"
#include "shooterentity.h"
#include "segmentgroup.h"

class Human : public ShooterEntity
{
public:
  Human(Vec2D const& position);
  Vec2D getPosition() const;
  Vec2D getVelocity() const;
  void setPosition(Vec2D const& newPosition);
  void setVelocity(Vec2D const& newVelocity);
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
protected:
  Vec2D position;
  Vec2D velocity;
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
  
  friend class CollisionHandler;
  
  Sprite sprite;
  
  static Image image;
};

#endif
