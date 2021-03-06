#ifndef GAMEWORLD_HH
#define GAMEWORLD_HH

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/collidableworld.h"
#include "ew/controllableworld.h"
#include "vectorterrainworld.h"
#include "ew/engine.h"

#include "glhck/glhck.h"

class GameWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
                  public ew::CollidableWorld, public ew::ControllableWorld,
                  public ew::VectorTerrainWorld
{
public:
  GameWorld();
  glhckCamera* getCamera();
  Vec2D const& getGravity() const;

private:
  glhckCamera* camera;
  Vec2D gravity;
};

#endif
