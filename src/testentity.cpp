#include "testentity.h"

Image TestEntity::image("img/icon.png", false);

TestEntity::TestEntity(Vec2D const& position) : Entity(), sprite(image, position), sprite2(image, Vec2D(0,0)), counter(0)
{
  image.load();
  sprite2.setParent(&sprite);
  sprite2.setPosition(Vec2D(100,0));
  sprite2.setRotation(0.5);
}

void TestEntity::render(Transformation const& view)
{
  sprite.render(view);
  sprite2.render(view);
}

void TestEntity::intent(float const delta)
{
  
}
void TestEntity::reaction(float const delta)
{
  
}
void TestEntity::update(float const delta)
{
  float const a = 0.5;
  float const b = 1.0;
  float const s0 = 0.5;
  float const s = 0.2;
  counter += delta;
  
  sprite.setScale(s0);
  sprite.rotate(delta/2);
  if(counter >= a && counter < b)
  {
    sprite.setScale((s0 + s) - (counter - a) / (b - a) * s);
  }
  
  if(counter > b)
  {
    counter -= b;
  }
}