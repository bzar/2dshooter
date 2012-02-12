#include "human.h"
#include "shooterworld.h"
#include <cmath>

Image Human::bodyImage("img/red_guy.png", false);
Image Human::rifleImage("img/rifle.png", false);
Image Human::leftHandImage("img/left_hand.png", false);
Image Human::rightHandImage("img/right_hand.png", false);

Human::Human(Vec2D const& position) : 
  MovingEntity(position), onGround(false), 
  body(bodyImage, position), 
  rifle(rifleImage, Vec2D(0, 3), &body),
  leftHand(leftHandImage, Vec2D(0, 0), &rifle), 
  rightHand(rightHandImage, Vec2D(0, 0), &rifle)
  
{
  bodyImage.load();
  rightHandImage.load();
  leftHandImage.load();
  rifleImage.load();
 
  body.setOrigin(Vec2D(0, -static_cast<int>(bodyImage.height()/2)));
  rifle.setOrigin(Vec2D(-8, 2));
}

void Human::render(Transformation const& view)
{
  body.setPosition(position);
  leftHand.render(view);
  body.render(view);
  rifle.render(view);
  rightHand.render(view);
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
  if(velocity.x < 0)
    body.setMirrorX(true);
  else if(velocity.x > 0)
    body.setMirrorX(false);
    
  position += velocity;
}

void Human::setOnGround(bool const value)
{
  onGround = value;
}

void Human::aimAt(Vec2D const& point)
{
  Vec2D direction = point - (position - body.getOrigin() + Vec2D(0, rifle.getOrigin().y + rifle.getPosition().y));
  if(body.getMirrorX())
    direction.x = -direction.x;
  float angle = direction.angle();
  rifle.setRotation(angle);
}

void Human::shoot(Bullet::Owner const owner)
{
  Vec2D bulletVelocity(800, 0);
  bulletVelocity.rotatei(rifle.getRotation());
  if(body.getMirrorX())
    bulletVelocity.x = -bulletVelocity.x;
  
  Bullet* bullet = new Bullet(rifle.getTransformation().transform(Vec2D(12, 3)), 
                              bulletVelocity, 0.5, owner);
  getWorld()->addEntity(bullet);
}

bool Human::CollisionHandler::handle(Segment const& segment, SegmentGroup const& group) const
{
  Vec2D relativeVelocity = human.getVelocity() - group.getVelocity();
  Segment relativeMotion(human.getPosition(), human.getPosition() + relativeVelocity);
  if(segment.intersects(relativeMotion))
  {
    Vec2D segmentDelta = segment.delta();
    if(segmentDelta.cross(relativeVelocity) < 0)
    {
      Vec2D collisionPoint = segment.intersectionPoint(relativeMotion);
      Vec2D segmentNormal = segmentDelta.normal().uniti();
      
      Vec2D tox = collisionPoint - human.getPosition();
      Vec2D rest = human.getPosition() + human.velocity - collisionPoint;

      human.setVelocity(tox.addi(rest.projectioni(segmentDelta))
                        .subtracti(segmentNormal.scale(0.1))
                        .addi(group.getVelocity()));
      
      if(segmentNormal.dot(human.shooterWorld->getGravity().unit()) > 0.5) {
        human.setOnGround(true);
      }
      
      return true;
    }
  }
  return false;
}