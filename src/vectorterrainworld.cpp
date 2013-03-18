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
