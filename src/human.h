#ifndef HUMAN_HH
#define HUMAN_HH

#include "framework.h"
#include "shooterworld.h"

class Human : public ShooterWorld::ShooterEntity
{
public:
  Human(Vec2D const& position);
  Vec2D getPosition() const;
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
private:
  Sprite sprite;
  Vec2D velocity;
  
  static Image image;
};

#endif
