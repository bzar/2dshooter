#include "vectorterrainphase.h"
#include "vectorterraincollidable.h"

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
      
      Segment closest;
      Vec2D closestCollisionPoint;
      float closestDistanceSquared = -1;
      collisions = false;
      
      // Find closest terrain collision
      segmentTree.query(relativeMotion, [&](Segment const& segment){
        if(segment.intersects(relativeMotion))
        {
          Vec2D segmentDelta = segment.delta();
          if(segmentDelta.cross(relativeVelocity) < 0)
          {
            Vec2D collisionPoint = segment.intersectionPoint(relativeMotion);
            collisions = true;
            float distanceSquared = (collisionPoint - relativeMotion.a).lengthSquared();
            if(closestDistanceSquared < 0 || distanceSquared < closestDistanceSquared)
            {
              if(segmentDelta.dot(v->getVelocity().normal()) != 0)
              {
                closestDistanceSquared = distanceSquared;
                closestCollisionPoint = collisionPoint;
                closest = segment;
              }
            }
          }
        }
        return false;
      });
      
      if(collisions)
      {
        v->vectorTerrainCollision(closest, closestCollisionPoint, delta);
      }
    } while(collisions);
  }
}
