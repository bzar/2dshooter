#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

#include "GL/glfw3.h"

namespace
{
  float const EPSILON = 0.00001;
  float const ONE_MINUS_EPSILON = 0.999999;
}

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
    velocity.x = -20;
  }
  else if(movingRight)
  {
    setPose("stand", false);
    setPose("stand-hands", false);
    setPose("walk", true);
    setPose("walk-hands", true);
    setFlipX(false);
    velocity.x = 20;
  }
  else
  {
    setPose("walk", false);
    setPose("walk-hands", false);
    setPose("stand", true);
    setPose("stand-hands", true);        
    velocity.x = 0;
  }
  
  velocity.y -= 5 * 9.81 * delta;

  setPosition(getPosition() + velocity.scale(delta));
  PuppetEntity::update(delta);
}

void Human::control(ew::ControlContext* context)
{
  movingLeft = context->keyDown(GLFW_KEY_LEFT);
  movingRight = context->keyDown(GLFW_KEY_RIGHT);
  if(context->keyPush(GLFW_KEY_R))
    setPosition({0, 0});
}

Vec2D const& Human::getPosition()
{
  return PuppetEntity::getPosition();
}

Vec2D const& Human::getVelocity()
{
  return velocity;
}

bool Human::vectorTerrainCollision(Segment const& segment, Vec2D const& collisionPoint, float const timeDelta)
{
  Vec2D relativeVelocity = velocity.scale(timeDelta);
  Vec2D segmentDelta = segment.delta();
  Vec2D segmentNormal = segmentDelta.normal().uniti();
  Vec2D rest = getPosition() - collisionPoint;
  
  relativeVelocity = rest.projectioni(segmentDelta).scalei(ONE_MINUS_EPSILON);
  setPosition(collisionPoint + relativeVelocity + segmentNormal.scale(-EPSILON));
  velocity = relativeVelocity.scale(1.0f/timeDelta);
  return true;
}
