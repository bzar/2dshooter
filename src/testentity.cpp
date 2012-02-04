#include "testentity.h"

Image TestEntity::image("img/icon.png", false);

TestEntity::TestEntity(Vec2D const& position) : World::Entity(), sprite(image, position), sprite2(image, Vec2D(0,0))
{
  image.load();
  sprite2.setParent(&sprite);
  sprite2.setPosition(Vec2D(100,0));
  sprite2.setRotation(0.5);
}

void TestEntity::render(Screen const& screen)
{
  sprite.render(screen);
  sprite2.render(screen);
}

void TestEntity::intent(float const delta)
{
  
}
void TestEntity::reaction(float const delta)
{
  
}
void TestEntity::update(float const delta)
{
  counter += delta;
  if(counter >= 0.2)
  {
    sprite.setScale(1.5 - (counter - 0.2) * 5);
  }
  
  if(counter > 0.3)
  {
    counter -= 0.3;
  }
}