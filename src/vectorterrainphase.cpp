#include "vectorterrainphase.h"
#include "vectorterraincollidable.h"

void ew::VectorTerrainPhase::execute(float const delta)
{
  SegmentTree const& segmentTree = world->getSegmentTree();
  
  for(VectorTerrainCollidable* v : world->getVectorTerrainCollidables())
  {
    // handle terrain collisions
  }
}
