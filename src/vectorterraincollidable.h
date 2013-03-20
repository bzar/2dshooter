#ifndef VECTORTERRAINCOLLIDABLE_HH
#define VECTORTERRAINCOLLIDABLE_HH

#include "ew/entity.h"
#include "vectorterrainworld.h"
#include "util/vec2d.h"

namespace ew
{
  class VectorTerrainCollidable : public virtual Entity
  {
  public:
    VectorTerrainCollidable(VectorTerrainWorld* world) : Entity(world), vectorTerrainWorld(world)
    {
      vectorTerrainWorld->registerVectorTerrainCollidable(this);
    }
    virtual ~VectorTerrainCollidable() { if(vectorTerrainWorld != nullptr) vectorTerrainWorld->unregisterVectorTerrainCollidable(this); }
    virtual Vec2D const& getPosition() = 0;
    virtual Vec2D const& getVelocity() = 0;
    virtual bool vectorTerrainCollision(Segment const& segment, Vec2D const& collisionPoint, float const timeDelta) = 0;
    void vectorTerrainCollidableUnregistered() { vectorTerrainWorld = nullptr; }

  private:
    VectorTerrainWorld* vectorTerrainWorld;
  };
};
#endif
