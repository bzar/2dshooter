#include "vectorterrainphase.h"
#include "vectorterraincollidable.h"
#include <iostream>

void ew::VectorTerrainPhase::execute(float const delta)
{
  SegmentTree const& segmentTree = world->getSegmentTree();

  for(VectorTerrainCollidable* v : world->getVectorTerrainCollidables())
  {
    bool collisions = false;
    do 
    {
      Vec2D relativeVelocity = v->getVelocity().scale(delta);
      Segment const relativeMotion{v->getPosition() - relativeVelocity, v->getPosition()};
      
      std::tuple<Segment, bool> collision = world->getFirstColliding(relativeMotion);
      collisions = std::get<1>(collision);
      if(collisions)
      {
        Segment closest = std::get<0>(collision);
        Vec2D closestCollisionPoint = closest.intersectionPoint(relativeMotion);
        v->vectorTerrainCollision(closest, closestCollisionPoint, delta);
      }
    } while(collisions);
  }
}

