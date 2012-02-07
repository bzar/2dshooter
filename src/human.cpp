#include "human.h"

Image Human::image("img/red_guy.png", false);

Human::Human(Vec2D const& position) : ShooterWorld::ShooterEntity(), sprite(image, position), velocity(0, 0)
{
  image.load();
}

Vec2D Human::getPosition() const
{
  return sprite.getPosition();
}

void Human::render(Transformation const& view)
{
  sprite.render(view);
}

void Human::intent(float const delta)
{
  velocity += getWorld()->getGravity().scale(delta);  
}
void Human::reaction(float const delta)
{
  
}
void Human::update(float const delta)
{
  sprite.setPosition(sprite.getPosition() + velocity);
}