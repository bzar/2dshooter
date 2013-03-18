#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

#include "GL/glfw3.h"

ew::UID const Human::ID = ew::getUID();
std::string const Human::PUPPET_FILE = "puppets/stickman.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), PuppetEntity(createPuppet(), world), ew::Controllable(world), ew::VectorTerrainCollidable(world), velocity(),
  movingLeft(false), movingRight(false)
{
}

Puppet Human::createPuppet()
{
  static auto value = qmlon::readFile(PUPPET_FILE);
  Puppet puppet;
  puppet.initialize(value);
  return puppet;
}

void Human::update(const float delta)
{
  if(movingLeft)
  {
    setPose("stand", false);
    setPose("stand-hands", false);
    setPose("walk", true);
    setPose("walk-hands", true);
    setFlipX(true);
    velocity.x = -10;
  }
  else if(movingRight)
  {
    setPose("stand", false);
    setPose("stand-hands", false);
    setPose("walk", true);
    setPose("walk-hands", true);
    setFlipX(false);
    velocity.x = 10;
  }
  else
  {
    setPose("walk", false);
    setPose("walk-hands", false);
    setPose("stand", true);
    setPose("stand-hands", true);        
    velocity.x = 0;
  }
  
  setPosition(getPosition() + velocity.scale(delta));
  velocity.y -= 9.81 * delta;
  PuppetEntity::update(delta);
}

void Human::control(ew::ControlContext* context)
{
  movingLeft = context->keyDown(GLFW_KEY_LEFT);
  movingRight = context->keyDown(GLFW_KEY_RIGHT);
}

Vec2D const& Human::getPosition()
{
  return PuppetEntity::getPosition();
}

Vec2D const& Human::getVelocity()
{
  return velocity;
}

bool Human::vectorTerrainCollision(Segment const& segment, float const timeDelta)
{
  /*
  Vec2D relativeVelocity = (human.getVelocity() - group.getVelocity()).scalei(timeDelta);
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
  */

  Vec2D relativeVelocity = getVelocity().scale(timeDelta);
  Segment relativeMotion(getPosition(), getPosition() + relativeVelocity);
  if(segment.intersects(relativeMotion))
  {
    Vec2D segmentDelta = segment.delta();
    if(segmentDelta.cross(relativeVelocity) < 0)
    {
      Vec2D collisionPoint = segment.intersectionPoint(relativeMotion);
      Vec2D segmentNormal = segmentDelta.normal().uniti();
      
      Vec2D tox = collisionPoint - getPosition();
      Vec2D rest = getPosition() + velocity - collisionPoint;

      velocity = tox.addi(rest.projectioni(segmentDelta))
                    .subtracti(segmentNormal.scale(0.1));
      return true;
    }
  }
  return false;
}
