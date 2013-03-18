#ifndef VECTORTERRAINCOLLIDABLE_HH
#define VECTORTERRAINCOLLIDABLE_HH

#include "ew/entity.h"
#include "vectorterrainworld.h"

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
    //virtual void update(float const delta) = 0;
    void vectorTerrainCollidableUnregistered() { vectorTerrainWorld = nullptr; }

  private:
    VectorTerrainWorld* vectorTerrainWorld;
  };
};
#endif
