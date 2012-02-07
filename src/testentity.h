#ifndef TEST_ENTITY_HH
#define TEST_ENTITY_HH

#include "framework.h"

class TestEntity : public Entity
{
public:
  TestEntity(Vec2D const& position);
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
private:
  Sprite sprite;
  Sprite sprite2;
  
  static Image image;
  float counter;
};

#endif
