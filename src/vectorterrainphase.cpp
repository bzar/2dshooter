#include "vectorterrainphase.h"
#include "vectorterraincollidable.h"

void ew::VectorTerrainPhase::execute(float const delta)
{
  SegmentTree const& segmentTree = world->getSegmentTree();
  
  for(VectorTerrainCollidable* v : world->getVectorTerrainCollidables())
  {
    Segment const segment{v->getPosition(), v->getPosition() + v->getVelocity()};
    segmentTree.query(segment, [v, delta](Segment const& s){
      return v->vectorTerrainCollision(s, delta);
    });
  }
}
