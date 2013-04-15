#include "human.h"
#include "bullet.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "GL/glfw3.h"

namespace
{
  float const EPSILON = 0.001;
  float const ONE_MINUS_EPSILON = 1.0 - EPSILON;
  float const ONE_PLUS_EPSILON = 1.0 + EPSILON;
}

ew::UID const Human::ID = ew::getUID();
std::string const Human::PUPPET_FILE = "puppets/commando.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), PuppetEntity(createPuppet(), world), ew::Controllable(world), ew::VectorTerrainCollidable(world), 
  world(world), movingLeft(false), movingRight(false), jumping(false), crouching(false),
  shooting(false), aiming(false), aimingUp(false), aimingDown(false), onGround(false),
  velocity(), shootDelay(0)
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
  if(movingLeft && !crouching)
  {
    setPose("stand", false);
    setPose("stand-hands", false);
    setPose("walk", true);
    setPose("walk-hands", true);
    setPose("crouch", false);
    setFlipX(true);
    velocity.x = -40;
  }
  else if(movingRight && !crouching)
  {
    setPose("stand", false);
    setPose("stand-hands", false);
    setPose("walk", true);
    setPose("walk-hands", true);
    setPose("crouch", false);
    setFlipX(false);
    velocity.x = 40;
  }
  else
  {
    setPose("walk", false);
    setPose("walk-hands", false);
    setPose("stand-hands", true);

    setPose(crouching ? "crouch" : "stand", true);
    setPose(!crouching ? "crouch" : "stand", false);
    velocity.x = 0;
  }

  if(jumping && onGround)
  {
    velocity.y = 200;
  }

  if(shootDelay > 0)
  {
    shootDelay -= delta;
  }

  Skeleton::Bone& aimBone = getPuppet().getSkeleton().getBone("arms-aim");
  float currentAim = aimBone.getAngle();

  if(aiming)
  {
    if(aimingUp && !aimingDown && currentAim < 0.20) {
      aimBone.changeAngle(0.5*delta);
    } else if(aimingDown && !aimingUp && currentAim > -0.20) {
      aimBone.changeAngle(-0.5*delta);
    }
    setPose("walk-hands", false);
    setPose("stand-hands", false);
    setPose("rifle-hip", false);
    setPose("rifle-aim", true);
    setPose("aim-front", false);
  }
  else
  {
    setPose("rifle-aim", false);
    setPose("aim-front", true);
  }

  if(shooting)
  {
    if(!aiming) {
      setPose("rifle-hip", true);
    }
    setPose("walk-hands", false);
    setPose("stand-hands", false);
  }
  else
  {
    setPose("rifle-hip", false);
  }
  if(shooting && shootDelay <= 0)
  {
    Skeleton::Bone const& weapon = getPuppet().getSkeleton().getBone("item-right");
    Vec2D bulletDirection = (weapon.getTip() - weapon.getBase()).uniti();
    Vec2D bulletPosition = getPosition() + weapon.getTip() + bulletDirection.scale(7);

    Bullet* bullet = new Bullet(world);
    bullet->setPosition(bulletPosition);
    bullet->setVelocity(bulletDirection * 200);
    shootDelay = 0.25;
  }

  velocity += world->getGravity() * delta;

  onGround = false;
  Vec2D const frameVelocity = velocity.scale(delta);
  setPosition(getPosition() + frameVelocity);
  PuppetEntity::update(delta);

  float const r = 10;
  glhckObject* camera = glhckCameraGetObject(world->getCamera());
  Vec2D cameraPosition(glhckObjectGetPosition(camera)->x, glhckObjectGetPosition(camera)->y);
  cameraPosition.x = cameraPosition.x - getPosition().x < -r ? getPosition().x - r : cameraPosition.x;
  cameraPosition.y = cameraPosition.y - getPosition().y < -r ? getPosition().y - r : cameraPosition.y;
  cameraPosition.x = cameraPosition.x - getPosition().x > r ? getPosition().x + r : cameraPosition.x;
  cameraPosition.y = cameraPosition.y - getPosition().y > r ? getPosition().y + r : cameraPosition.y;
  glhckObjectPositionf(camera, cameraPosition.x, cameraPosition.y, 150);
  glhckObjectTargetf(camera, cameraPosition.x, cameraPosition.y, 0);
  glhckCameraUpdate(world->getCamera());
}

void Human::control(ew::ControlContext* context)
{
  movingLeft = context->keyDown(GLFW_KEY_LEFT);
  movingRight = context->keyDown(GLFW_KEY_RIGHT);
  jumping = context->keyDown(GLFW_KEY_SPACE);
  crouching = context->keyDown(GLFW_KEY_Z);
  shooting = context->keyDown(GLFW_KEY_C);
  aiming = context->keyDown(GLFW_KEY_X);
  aimingUp = context->keyDown(GLFW_KEY_UP);
  aimingDown = context->keyDown(GLFW_KEY_DOWN);

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
