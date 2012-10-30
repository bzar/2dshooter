#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

ew::UID const Human::ID = ew::getUID();
std::string const Human::PUPPET_FILE = "puppets/stickman.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), PuppetEntity(createPuppet(), world)
{
}

Puppet Human::createPuppet()
{
  static auto value = qmlon::readFile(PUPPET_FILE);
  Puppet puppet;
  puppet.initialize(value);
  return puppet;
}
