#include "gameworld.h"
#include "human.h"

GameWorld::GameWorld() :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(), ew::CollidableWorld(),
  ew::ControllableWorld(), ew::VectorTerrainWorld(),
  camera(glhckCameraNew())
{
  Human* human = new Human(this);

  human->setPosition({0, 0});
  human->setPose("walk", true);
  human->setPose("walk-hands", true);
  
  glhckObjectPositionf(glhckCameraGetObject(camera), 0, 0, 100);
  glhckCameraUpdate(camera);
  
  const std::list<Segment> segments = {{{-200, -5}, {200, -5}}};
  SegmentTree segmentTree(segments);
  setSegmentTree(segmentTree);
}
