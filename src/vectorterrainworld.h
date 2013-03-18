#ifndef VECTORTERRAINWORLD_HH
#define VECTORTERRAINWORLD_HH

#include "ew/world.h"
#include "util/segmenttree.h"
#include <set>

namespace ew
{
  class VectorTerrainCollidable;

  class VectorTerrainWorld : public virtual World
  {
  public:
    VectorTerrainWorld(SegmentTree const& segmentTree = SegmentTree({}));
    virtual ~VectorTerrainWorld();

    void registerVectorTerrainCollidable(VectorTerrainCollidable* updatable);
    void unregisterVectorTerrainCollidable(VectorTerrainCollidable* updatable);
    std::set<VectorTerrainCollidable*> const& getVectorTerrainCollidables();
    void setSegmentTree(SegmentTree const& segmentTree);
    SegmentTree const& getSegmentTree() const;

  private:
    std::set<VectorTerrainCollidable*> vectorTerrainCollidablesToInsert;
    std::set<VectorTerrainCollidable*> vectorTerrainCollidables;
    
    SegmentTree segmentTree;
  };
};
#endif
