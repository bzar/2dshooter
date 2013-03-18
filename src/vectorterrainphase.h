#ifndef VECTORTERRAINPHASE_HH
#define VECTORTERRAINPHASE_HH

#include "ew/phase.h"
#include "vectorterrainworld.h"

namespace ew
{
  class VectorTerrainPhase : public Phase
  {
  public:
    VectorTerrainPhase(VectorTerrainWorld* world) : world(world) {}
    void execute(float const delta);

  private:
    VectorTerrainWorld* world;
  };
};
#endif
