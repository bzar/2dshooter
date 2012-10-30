#include "gameworld.h"
#include "human.h"

GameWorld::GameWorld() :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(), ew::CollidableWorld(),
  ew::ControllableWorld(),
  camera(glhckCameraNew())
{
  Human* human1 = new Human(this);
  Human* human2 = new Human(this);
  human1->setPosition({5, 0});
  human2->setPosition({-5, 0});
  
  human1->getPuppet().getSkeleton().getPose("walk").activate();
  human1->getPuppet().getSkeleton().getPose("walk-hands").activate();
  
  human2->getPuppet().getSkeleton().getPose("stand").activate();
  human2->getPuppet().getSkeleton().getPose("stand-hands").activate();

  glhckObjectPositionf(glhckCameraGetObject(camera), 0, 0, 100);
  glhckCameraUpdate(camera);
}
