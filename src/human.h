#ifndef HUMAN_HH
#define HUMAN_HH

#include "puppetentity.h"
#include "ew/controllable.h"
#include <string>

class Human : public PuppetEntity, public ew::Controllable
{
public:
  Human(GameWorld* world);

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  virtual void control(ew::ControlContext* context);
  
private:
  static std::string const PUPPET_FILE;
  static Puppet createPuppet();
};

#endif