#include "gameworld.h"
#include "human.h"

GameWorld::GameWorld() :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(), ew::CollidableWorld(),
  ew::ControllableWorld(),
  camera(glhckCameraNew())
{
  Human* human = new Human(this);

  human->setPosition({0, 0});
  
  glhckObjectPositionf(glhckCameraGetObject(camera), 0, 0, 100);
  glhckCameraUpdate(camera);
}
