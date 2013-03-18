#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

#include "GL/glfw3.h"

ew::UID const Human::ID = ew::getUID();
std::string const Human::PUPPET_FILE = "puppets/stickman.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), PuppetEntity(createPuppet(), world), ew::Controllable(world),
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
    setPosition(getPosition() + Vec2D(-10, 0).scale(delta));
  }
  else if(movingRight)
  {
    setPose("stand", false);
    setPose("stand-hands", false);
    setPose("walk", true);
    setPose("walk-hands", true);
    setFlipX(false);
    setPosition(getPosition() + Vec2D(10, 0).scale(delta));
  }
  else
  {
    setPose("walk", false);
    setPose("walk-hands", false);
    setPose("stand", true);
    setPose("stand-hands", true);        
  }
  
  PuppetEntity::update(delta);
}

void Human::control(ew::ControlContext* context)
{
  movingLeft = context->keyDown(GLFW_KEY_LEFT);
  movingRight = context->keyDown(GLFW_KEY_RIGHT);
}
