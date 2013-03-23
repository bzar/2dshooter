#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

#include "GL/glfw3.h"

namespace
{
  float const EPSILON = 0.001;
  float const ONE_MINUS_EPSILON = 0.999;
  float const ONE_PLUS_EPSILON = 1.001;
}

ew::UID const Human::ID = ew::getUID();
std::string const Human::PUPPET_FILE = "puppets/stickman.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), PuppetEntity(createPuppet(), world), ew::Controllable(world), ew::VectorTerrainCollidable(world), 
  world(world), velocity(), movingLeft(false), movingRight(false), jumping(false), onGround(false)
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
    velocity.x = -40;
  }
  else if(movingRight)
  {
    setPose("stand", false);
    setPose("stand-hands", false);
    setPose("walk", true);
    setPose("walk-hands", true);
    setFlipX(false);
    velocity.x = 40;
  }
  else
  {
    setPose("walk", false);
    setPose("walk-hands", false);
    setPose("stand", true);
    setPose("stand-hands", true);        
    velocity.x = 0;
  }

  if(jumping && onGround)
  {
    velocity.y = 200;
  }
  
  velocity += world->getGravity() * delta;

  onGround = false;
  setPosition(getPosition() + velocity.scale(delta));
  PuppetEntity::update(delta);

  glhckObject* camera = glhckCameraGetObject(world->getCamera());
  glhckObjectPositionf(camera, getPosition().x, getPosition().y, 150);
  glhckObjectTargetf(camera, getPosition().x, getPosition().y, 0);
  glhckCameraUpdate(world->getCamera());
}

void Human::control(ew::ControlContext* context)
{
  movingLeft = context->keyDown(GLFW_KEY_LEFT);
  movingRight = context->keyDown(GLFW_KEY_RIGHT);
  jumping = context->keyDown(GLFW_KEY_SPACE);
  
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

  onGround |= segmentDelta.cross(world->getGravity()) < 0;

  if(world->getCollideCount({collisionPoint, collisionPoint + segmentNormal.scale(-EPSILON)}) > 0)
  {
    setPosition(getPosition() - relativeVelocity);
    velocity = Vec2D();
    return false;
  }
  else
  {
    Vec2D rest = getPosition() - collisionPoint;
    relativeVelocity = rest.projectioni(segmentDelta);
    velocity = relativeVelocity.scale(1.0f/timeDelta);
    setPosition(collisionPoint + relativeVelocity.scale(ONE_MINUS_EPSILON) + segmentNormal.scale(-EPSILON));
  }
  return true;
}
