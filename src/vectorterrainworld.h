#ifndef VECTORTERRAINWORLD_HH
#define VECTORTERRAINWORLD_HH

#include "ew/world.h"
#include "util/segmenttree.h"
#include <set>
#include <tuple>

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

    SegmentTree::SegmentList getColliding(Segment const& motion);
    std::tuple<Segment, bool> getFirstColliding(Segment const& motion);
    int getCollideCount(Segment const& motion);

  private:
    std::set<VectorTerrainCollidable*> vectorTerrainCollidablesToInsert;
    std::set<VectorTerrainCollidable*> vectorTerrainCollidables;
    
    SegmentTree segmentTree;
  };
};
#endif
