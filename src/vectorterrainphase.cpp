#include "vectorterrainphase.h"
#include "vectorterraincollidable.h"
#include <iostream>

void ew::VectorTerrainPhase::execute(float const delta)
{
  for(VectorTerrainCollidable* v : world->getVectorTerrainCollidables())
  {
    bool collisions = false;
    do 
    {
      Segment const relativeMotion(v->getPosition() - v->getVelocity().scale(delta), v->getPosition());
      
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

