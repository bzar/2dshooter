#include "vectorterrainworld.h"
#include "vectorterraincollidable.h"

ew::VectorTerrainWorld::VectorTerrainWorld(const SegmentTree& segmentTree) :
  World(), vectorTerrainCollidables(), vectorTerrainCollidablesToInsert(),
  segmentTree(segmentTree)
{
  onMaintenance([&]() {
    if(!vectorTerrainCollidablesToInsert.empty())
    {
      vectorTerrainCollidables.insert(vectorTerrainCollidablesToInsert.begin(), vectorTerrainCollidablesToInsert.end());
      vectorTerrainCollidablesToInsert.clear();
    }
  });
}

ew::VectorTerrainWorld::~VectorTerrainWorld()
{
  for(VectorTerrainCollidable* u : vectorTerrainCollidables)
  {
    u->vectorTerrainCollidableUnregistered();
  }
}


void ew::VectorTerrainWorld::registerVectorTerrainCollidable(VectorTerrainCollidable* vectorTerrainCollidable)
{
  vectorTerrainCollidablesToInsert.insert(vectorTerrainCollidable);
}

void ew::VectorTerrainWorld::unregisterVectorTerrainCollidable(VectorTerrainCollidable* vectorTerrainCollidable)
{
  vectorTerrainCollidablesToInsert.erase(vectorTerrainCollidable);
  vectorTerrainCollidables.erase(vectorTerrainCollidable);
  vectorTerrainCollidable->vectorTerrainCollidableUnregistered();
}

std::set<ew::VectorTerrainCollidable*> const& ew::VectorTerrainWorld::getVectorTerrainCollidables()
{
  return vectorTerrainCollidables;
}

void ew::VectorTerrainWorld::setSegmentTree(const SegmentTree& newSegmentTree)
{
  segmentTree = newSegmentTree;
}

SegmentTree const& ew::VectorTerrainWorld::getSegmentTree() const
{
  return segmentTree;
}

SegmentTree::SegmentList ew::VectorTerrainWorld::getColliding(const Segment &motion)
{
  SegmentTree::SegmentList result;
  segmentTree.query(motion, [&](Segment const& segment){
    if(segment.intersects(motion))
    {
      if(segment.delta().cross(motion.delta()) <= 0)
      {
        result.push_back(segment);
      }
    }
    return false;
  });

  return result;
}

std::tuple<Segment, bool> ew::VectorTerrainWorld::getFirstColliding(const Segment &motion)
{
  Segment closest;
  bool collisions = false;
  float closestDistanceSquared = -1;

  // Find closest terrain collision
  segmentTree.query(motion, [&](Segment const& segment){
    if(segment.intersects(motion))
    {
      if(segment.delta().cross(motion.delta()) <= 0)
      {
        Vec2D collisionPoint = segment.intersectionPoint(motion);
        float distanceSquared = (collisionPoint - motion.a).lengthSquared();
        if(closestDistanceSquared < 0 || distanceSquared < closestDistanceSquared)
        {
          closestDistanceSquared = distanceSquared;
          closest = segment;
          collisions = true;
        }
      }
    }
    return false;
  });

  return std::make_tuple(closest, collisions);
}

int ew::VectorTerrainWorld::getCollideCount(const Segment &motion)
{
  int result = 0;
  segmentTree.query(motion, [&](Segment const& segment){
    if(segment.intersects(motion))
    {
      if(segment.delta().cross(motion.delta()) <= 0)
      {
        result += 1;
      }
    }
    return false;
  });

  return result;
}
