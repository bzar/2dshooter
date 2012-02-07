#include "human.h"
#include "shooterworld.h"

Image Human::image("img/red_guy.png", false);

Human::Human(Vec2D const& position) : ShooterEntity(), sprite(image, position), velocity(0, 0)
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
  std::set<SegmentGroup*> const& terrain = getWorld()->getTerrain();
  CollisionHandler handler(this);
  Segment s(sprite.getPosition() - Vec2D(0, image.height()/2), sprite.getPosition() - Vec2D(0, image.height()/2) + velocity);
  bool quit = false;
  for(std::set<SegmentGroup*>::const_iterator i = terrain.begin(); !quit && i != terrain.end(); ++i)
  {
    quit = (*i)->query(s, handler);
  }
}
void Human::update(float const delta)
{
  sprite.setPosition(sprite.getPosition() + velocity);
}

bool Human::CollisionHandler::handle(Segment const& segment) const
{
  Log::debug() << "Collision!";
  human->velocity.negi();
  return false;
}