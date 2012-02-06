#include "human.h"

Image Human::image("img/red_guy.png", false);

Human::Human(Vec2D const& position) : ShooterWorld::ShooterEntity(), sprite(image, position), velocity(0, 0)
{
  image.load();
}

void Human::render(Screen const& screen)
{
  sprite.render(screen);
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