#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

#include "GL/glfw3.h"

ew::UID const Human::ID = ew::getUID();
std::string const Human::PUPPET_FILE = "puppets/stickman.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), PuppetEntity(createPuppet(), world), ew::Controllable(world)
{
}

Puppet Human::createPuppet()
{
  static auto value = qmlon::readFile(PUPPET_FILE);
  Puppet puppet;
  puppet.initialize(value);
  return puppet;
}

void Human::control(ew::ControlContext* context)
{
  if(context->keyPush(GLFW_KEY_D))
  {
    setPose("walk", true);
  }
  
  if(context->keyPush(GLFW_KEY_C))
  {
    setPose("walk", false);
  }
  
  if(context->keyPush(GLFW_KEY_F))
  {
    setPose("walk-hands", true);
  }
  
  if(context->keyPush(GLFW_KEY_V))
  {
    setPose("walk-hands", false);
  }
  
  if(context->keyPush(GLFW_KEY_A))
  {
    setPose("stand", true);
  }
  
  if(context->keyPush(GLFW_KEY_Z))
  {
    setPose("stand", false);
  }
  
  if(context->keyPush(GLFW_KEY_S))
  {
    setPose("stand-hands", true);
  }
  
  if(context->keyPush(GLFW_KEY_X))
  {
    setPose("stand-hands", false);
  }
  
  if(context->keyPush(GLFW_KEY_W))
  {
    setFlipX(!getPuppet().getFlipX());
  }
  
  if(context->keyPush(GLFW_KEY_E))
  {
    setFlipY(!getPuppet().getFlipY());
  }

}
