#include "human.h"
#include "shooterworld.h"

Image Human::image("img/red_guy.png", false);

Human::Human(Vec2D const& position) : 
  ShooterEntity(), position(position), velocity(0, 0), onGround(false), sprite(image, position)
{
  image.load();
}

Vec2D Human::getPosition() const
{
  return position;
}
Vec2D Human::getVelocity() const
{
  return velocity;
}
void Human::setPosition(Vec2D const& newPosition)
{
  position = newPosition;
}
void Human::setVelocity(Vec2D const& newVelocity)
{
  velocity = newVelocity;
}

void Human::render(Transformation const& view)
{
  sprite.setPosition(position + Vec2D(0, image.height()/2));
  sprite.render(view);
}

void Human::intent(float const delta)
{
  onGround = false;
  velocity += getWorld()->getGravity().scale(delta);  
}
void Human::reaction(float const delta)
{
  std::set<SegmentGroup*> const& terrain = getWorld()->getTerrain();
  CollisionHandler handler(*this);
  bool collisions = true;
  while(collisions)
  {
    Segment s(position, position + velocity);
    collisions = false;
    for(std::set<SegmentGroup*>::const_iterator i = terrain.begin(); !collisions && i != terrain.end(); ++i)
    {
      collisions = (*i)->query(s, handler);
    }
  }
}
void Human::update(float const delta)
{
  position += velocity;
}

bool Human::CollisionHandler::handle(Segment const& segment, SegmentGroup const& group) const
{
  Vec2D relativeVelocity = human.velocity - group.getVelocity();
  Segment relativeMotion(human.position, human.position + relativeVelocity);
  if(segment.intersects(relativeMotion))
  {
    Vec2D segmentDelta = segment.delta();
    if(segmentDelta.cross(relativeVelocity) < 0)
    {
      Vec2D collisionPoint = segment.intersectionPoint(relativeMotion);
      Vec2D segmentNormal = segmentDelta.normal().uniti();
      
      Vec2D tox = collisionPoint - human.position;
      Vec2D rest = human.position + human.velocity - collisionPoint;

      human.velocity = tox.addi(rest.projectioni(segmentDelta))
                          .subtracti(segmentNormal.scale(0.1))
                          .addi(group.getVelocity());
      
      if(segmentNormal.dot(human.shooterWorld->getGravity().unit()) > 0.5) {
        human.onGround = true;
      }
      
      return true;
    }
  }
  return false;
}