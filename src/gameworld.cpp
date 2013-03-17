#include "gameworld.h"
#include "human.h"

GameWorld::GameWorld() :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(), ew::CollidableWorld(),
  ew::ControllableWorld(),
  camera(glhckCameraNew())
{
  Human* human1 = new Human(this);
  Human* human2 = new Human(this);
  Human* human3 = new Human(this);
  Human* human4 = new Human(this);
  human1->setPosition({15, 5});
  human2->setPosition({-15, 5});
  human3->setPosition({15, -15});
  human4->setPosition({-15, -15});
  
  human1->getPuppet().getSkeleton().getPose("walk").activate();
  human1->getPuppet().getSkeleton().getPose("walk-hands").activate();
  
  human2->getPuppet().getSkeleton().getPose("stand").activate();
  human2->getPuppet().getSkeleton().getPose("stand-hands").activate();

  human3->getPuppet().getSkeleton().getPose("stand").activate();
  human3->getPuppet().getSkeleton().getPose("stand-hands").activate();

  human4->getPuppet().getSkeleton().getPose("walk").activate();
  human4->getPuppet().getSkeleton().getPose("walk-hands").activate();
  
  glhckObjectPositionf(glhckCameraGetObject(camera), 0, 0, 100);
  glhckCameraUpdate(camera);
}
