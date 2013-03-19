#include "gameworld.h"
#include "human.h"
#include "vectorterrain.h"

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
  
  const std::list<Segment> segments = {
    {{-20, -5}, {20, -5}},
    {{-20, -15}, {20, -15}},
    {{-20, -5}, {-20, -15}},
    {{20, -15}, {30, -25}},
    {{30, -25}, {40, -15}},
    {{40, -15}, {45, -30}},
    {{45, -30}, {50, -15}},
    
  };
  SegmentTree segmentTree(segments);
  setSegmentTree(segmentTree);
  new VectorTerrain(this, segments, 0, 0);
}
