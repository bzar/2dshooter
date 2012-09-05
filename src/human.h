#ifndef HUMAN_HH
#define HUMAN_HH

#include "ew/renderable.h"
#include "ew/updatable.h"
#include "skeleton.h"
#include "gameworld.h"
#include "GL/glhck.h"

#include <string>

class Human : public ew::Renderable, public ew::Updatable
{
public:
  Human(GameWorld* world);
  ~Human();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
  void update(float const delta);

private:
  static std::string const SKELETON_FILE;
  glhckObject* debugLines;

  Skeleton skeleton;
};

#endif