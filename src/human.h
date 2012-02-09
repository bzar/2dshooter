#ifndef HUMAN_HH
#define HUMAN_HH

#include "framework.h"
#include "shooterentity.h"

class Human : public ShooterEntity
{
public:
  Human(Vec2D const& position);
  Vec2D getPosition() const;
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
private:
  class CollisionHandler : public SegmentTree::ResultHandler
  {
  public:
    CollisionHandler(Human* human) : human(human) {}
    virtual bool handle(Segment const& segment) const;
  private:
    Human* human;
  };
  
  friend class CollisionHandler;
  
  Sprite sprite;
  Vec2D velocity;
  
  static Image image;
};

#endif