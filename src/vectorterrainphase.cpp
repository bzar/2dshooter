#include "vectorterrainphase.h"
#include "vectorterraincollidable.h"

void ew::VectorTerrainPhase::execute(float const delta)
{
  SegmentTree const& segmentTree = world->getSegmentTree();
  
  for(VectorTerrainCollidable* v : world->getVectorTerrainCollidables())
  {
    bool collisions = true;
    while(collisions) {
      Segment const segment{v->getPosition() - v->getVelocity().scale(delta), v->getPosition()};
      collisions = segmentTree.query(segment, [v, delta](Segment const& s){
        return v->vectorTerrainCollision(s, delta);
      });
    }
  }
}
