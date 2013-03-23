#include "gameworld.h"
#include "human.h"
#include "vectorterrain.h"
#include "level.h"

GameWorld::GameWorld() :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(), ew::CollidableWorld(),
  ew::ControllableWorld(), ew::VectorTerrainWorld(),
  camera(glhckCameraNew())
{
  Human* human = new Human(this);
  glhckObjectPositionf(glhckCameraGetObject(camera), 0, 0, 300);
  glhckCameraRange(camera, 1.0f, 1000.0f);
  //glhckCameraProjection(camera, GLHCK_PROJECTION_ORTHOGRAPHIC);
  glhckCameraUpdate(camera);
  
  Level level("levels/test.qmlon");
  const std::list<Segment> segments = level.getSegments();
  human->setPosition(level.getStartPosition());
  SegmentTree segmentTree(segments);
  setSegmentTree(segmentTree);
  new VectorTerrain(this, segments, 0, 0);
}

glhckCamera* GameWorld::getCamera()
{
  return camera;
}
