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
    getPuppet().getSkeleton().getPose("walk").reset();
    getPuppet().getSkeleton().getPose("walk").activate();
  }
  
  if(context->keyPush(GLFW_KEY_C))
  {
    getPuppet().getSkeleton().getPose("walk").deactivate();
  }
  
  if(context->keyPush(GLFW_KEY_F))
  {
    getPuppet().getSkeleton().getPose("walk-hands").reset();
    getPuppet().getSkeleton().getPose("walk-hands").activate();
  }
  
  if(context->keyPush(GLFW_KEY_V))
  {
    getPuppet().getSkeleton().getPose("walk-hands").deactivate();
  }
  
  if(context->keyPush(GLFW_KEY_A))
  {
    getPuppet().getSkeleton().getPose("stand").reset();
    getPuppet().getSkeleton().getPose("stand").activate();
  }
  
  if(context->keyPush(GLFW_KEY_Z))
  {
    getPuppet().getSkeleton().getPose("stand").deactivate();
  }
  
  if(context->keyPush(GLFW_KEY_S))
  {
    getPuppet().getSkeleton().getPose("stand-hands").reset();
    getPuppet().getSkeleton().getPose("stand-hands").activate();
  }
  
  if(context->keyPush(GLFW_KEY_X))
  {
    getPuppet().getSkeleton().getPose("stand-hands").deactivate();
  }
}
