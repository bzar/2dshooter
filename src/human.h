#ifndef HUMAN_HH
#define HUMAN_HH

#include "puppetentity.h"
#include "ew/controllable.h"
#include "vectorterraincollidable.h"
#include <string>

class Human : public PuppetEntity, public ew::Controllable, public ew::VectorTerrainCollidable
{
public:
  Human(GameWorld* world);

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  virtual void update(float const delta);
  virtual void control(ew::ControlContext* context);
  
  virtual Vec2D const& getPosition();
  virtual Vec2D const& getVelocity();
  virtual bool vectorTerrainCollision(const Segment& segment, Vec2D const& collisionPoint, const float timeDelta);
  
private:
  static std::string const PUPPET_FILE;
  static Puppet createPuppet();
  
  GameWorld* world;
  bool movingLeft;
  bool movingRight;
  bool jumping;
  bool crouching;
  bool aiming;
  bool shooting;
  bool onGround;
  
  float shootDelay;
  Vec2D velocity;
};

#endif
