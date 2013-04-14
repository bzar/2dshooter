#include "gameworld.h"
#include "human.h"
#include "vectorterrain.h"
#include "levelterrain.h"
#include "parallaxbackground.h"
#include "level.h"

GameWorld::GameWorld() :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(), ew::CollidableWorld(),
  ew::ControllableWorld(), ew::VectorTerrainWorld(),
  camera(glhckCameraNew()), gravity(0, 50 * -9.81)
{
  Human* human = new Human(this);
  glhckObjectPositionf(glhckCameraGetObject(camera), 0, 0, 300);
  glhckCameraRange(camera, 1.0f, 1000.0f);
  glhckCameraProjection(camera, GLHCK_PROJECTION_ORTHOGRAPHIC);
  glhckCameraUpdate(camera);
  
  Level level("levels/test.qmlon");

  std::list<Segment> segments;
  for(Level::Line const& line : level.getLines())
  {
    auto a = line.vertices.begin();
    auto b = line.vertices.begin();
    ++b;
    while(b != line.vertices.end())
    {
      segments.push_back({*a, *b});
      ++a;
      ++b;
    }
  }

  human->setPosition(level.getStartPosition());
  SegmentTree segmentTree(segments);
  setSegmentTree(segmentTree);
  //new VectorTerrain(this, segments, 0, 0);
  new LevelTerrain(this, level, -1, 0);
  new ParallaxBackground(this, camera, "img/test_background.png", 0, -1);
}

glhckCamera* GameWorld::getCamera()
{
  return camera;
}

const Vec2D &GameWorld::getGravity() const
{
  return gravity;
}
