#ifndef SEGMENT_GROUP_HH
#define SEGMENT_GROUP_HH

#include "framework.h"
#include "shooterworld.h"
#include <list>

class SegmentGroup : public ShooterWorld::ShooterEntity
{
public:
  typedef std::list<Segment> SegmentList;

  SegmentGroup(Vec2D const& position, SegmentList const& segments);
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
private:
  void drawArrow(Transformation const& view, Vec2D const& base, Vec2D const& tip, float const r, float const g, float const b, float const lineWidth, float const tipLength, float const tipWidth);
  
  Vec2D position;
  SegmentList segments;
  SegmentTree segmentTree;
};

#endif
