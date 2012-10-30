#ifndef HUMAN_HH
#define HUMAN_HH

#include "puppetentity.h"
#include <string>

class Human : public PuppetEntity
{
public:
  Human(GameWorld* world);

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

private:
  static std::string const PUPPET_FILE;
  static Puppet createPuppet();
};

#endif